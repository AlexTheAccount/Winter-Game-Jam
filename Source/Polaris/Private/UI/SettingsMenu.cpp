// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SettingsMenu.h"
#include "UI/UIManager.h"

#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "Components/CheckBox.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Misc/ConfigCacheIni.h"
#include "Blueprint/WidgetTree.h"
#include "GameFramework/GameUserSettings.h"
#include "Widgets/SWidget.h"
#include "Framework/Application/SlateApplication.h"
#include <EnhancedInputSubsystems.h>
#include "Blueprint/UserWidget.h"

// Actions to expose for rebinding
static const TArray<FName> ActionsToExpose =
{
    TEXT("Jump"),
    TEXT("Accelerate"),
    TEXT("Brake"),
    TEXT("Steer"),
    TEXT("Dismount"),
};

void USettingsMenu::NativeConstruct()
{
    Super::NativeConstruct();

    // Wire delegates and UI setup
    BindWidgetEvents();
    PopulateVideoOptions();
    SetupInputBindings();

    // Load current settings into widgets
    LoadSettingsIntoWidgets();
}

void USettingsMenu::NativeDestruct()
{
    // Unbind events and cleanup
    UnbindWidgetEvents();
    CleanupRebindRows();

    // Flush config changes
    if (GConfig)
    {
        GConfig->Flush(false, GGameIni);
    }

    Super::NativeDestruct();
}

void USettingsMenu::BindWidgetEvents()
{
    if (BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &USettingsMenu::OnBackClicked);
    }

    // Audio sliders
    if (MasterVolumeSlider)
    {
        MasterVolumeSlider->OnValueChanged.AddDynamic(this, &USettingsMenu::OnMasterVolumeChanged);
    }
    if (SFXVolumeSlider)
    {
        SFXVolumeSlider->OnValueChanged.AddDynamic(this, &USettingsMenu::OnSFXVolumeChanged);
    }
    if (MusicVolumeSlider)
    {
        MusicVolumeSlider->OnValueChanged.AddDynamic(this, &USettingsMenu::OnMusicVolumeChanged);
    }

    // Video widgets
    if (ResolutionCombo)
    {
        ResolutionCombo->OnSelectionChanged.AddDynamic(this, &USettingsMenu::OnResolutionChanged);
    }
    if (WindowModeCombo)
    {
        WindowModeCombo->OnSelectionChanged.AddDynamic(this, &USettingsMenu::OnWindowModeChanged);
    }
    if (VSyncCheckBox)
    {
        VSyncCheckBox->OnCheckStateChanged.AddDynamic(this, &USettingsMenu::OnVSyncChanged);
    }
    if (BrightnessSlider)
    {
        BrightnessSlider->OnValueChanged.AddDynamic(this, &USettingsMenu::OnBrightnessChanged);
    }

    // Reset defaults
    if (ResetDefaultsButton)
    {
        ResetDefaultsButton->OnClicked.AddDynamic(this, &USettingsMenu::OnResetToDefaultsClicked);
    }
}

void USettingsMenu::UnbindWidgetEvents()
{
    if (BackButton)
    {
        BackButton->OnClicked.RemoveDynamic(this, &USettingsMenu::OnBackClicked);
    }

    if (MasterVolumeSlider) MasterVolumeSlider->OnValueChanged.RemoveDynamic(this, &USettingsMenu::OnMasterVolumeChanged);
    if (SFXVolumeSlider) SFXVolumeSlider->OnValueChanged.RemoveDynamic(this, &USettingsMenu::OnSFXVolumeChanged);
    if (MusicVolumeSlider) MusicVolumeSlider->OnValueChanged.RemoveDynamic(this, &USettingsMenu::OnMusicVolumeChanged);

    if (ResolutionCombo) ResolutionCombo->OnSelectionChanged.RemoveDynamic(this, &USettingsMenu::OnResolutionChanged);
    if (WindowModeCombo) WindowModeCombo->OnSelectionChanged.RemoveDynamic(this, &USettingsMenu::OnWindowModeChanged);
    if (VSyncCheckBox) VSyncCheckBox->OnCheckStateChanged.RemoveDynamic(this, &USettingsMenu::OnVSyncChanged);
    if (BrightnessSlider) BrightnessSlider->OnValueChanged.RemoveDynamic(this, &USettingsMenu::OnBrightnessChanged);

    if (ResetDefaultsButton) ResetDefaultsButton->OnClicked.RemoveDynamic(this, &USettingsMenu::OnResetToDefaultsClicked);
}

void USettingsMenu::PopulateVideoOptions()
{
    if (ResolutionCombo && ResolutionCombo->GetOptionCount() == 0)
    {
        ResolutionCombo->AddOption(TEXT("1920x1080"));
        ResolutionCombo->AddOption(TEXT("1600x900"));
        ResolutionCombo->AddOption(TEXT("1280x720"));
    }

    if (WindowModeCombo && WindowModeCombo->GetOptionCount() == 0)
    {
        WindowModeCombo->AddOption(TEXT("Fullscreen"));
        WindowModeCombo->AddOption(TEXT("Windowed"));
        WindowModeCombo->AddOption(TEXT("Borderless"));
    }
}

void USettingsMenu::SetupInputBindings()
{
    RebindButtons.Empty();
    RebindKeyLabels.Empty();

    if (InputBindingsContainer)
    {
        InputBindingsContainer->ClearChildren();

        for (const FName& Action : ActionsToExpose)
        {
            UTextBlock* KeyLabel = nullptr;
            UButton* RebindBtn = nullptr;
            UHorizontalBox* Row = CreateInputBindingRow(Action, KeyLabel, RebindBtn);

            if (Row)
            {
                InputBindingsContainer->AddChild(Row);
            }

            if (RebindBtn)
            {
                RebindBtn->OnClicked.AddDynamic(this, &USettingsMenu::OnRebindButtonClicked);
                RebindButtons.Add(RebindBtn);
            }

            if (KeyLabel)
            {
                RebindKeyLabels.Add(KeyLabel);
            }
        }
    }
}

UHorizontalBox* USettingsMenu::CreateInputBindingRow(const FName& ActionName, UTextBlock*& OutKeyLabel, UButton*& OutButton)
{
    OutKeyLabel = nullptr;
    OutButton = nullptr;

    if (WidgetTree == nullptr) return nullptr;

    // create a horizontal layout row
    UHorizontalBox* Row = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
    if (Row == nullptr) return nullptr;

    // Label
    UTextBlock* Label = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
    Label->SetText(FText::FromName(ActionName));
    UHorizontalBoxSlot* LabelSlot = Row->AddChildToHorizontalBox(Label);
    if (LabelSlot) LabelSlot->SetPadding(FMargin(6.0f, 2.0f));

    // Current key label
    UTextBlock* KeyText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
    KeyText->SetText(FText::FromString(TEXT("...")));
    UHorizontalBoxSlot* KeySlot = Row->AddChildToHorizontalBox(KeyText);
    if (KeySlot) KeySlot->SetPadding(FMargin(6.0f, 2.0f));
    OutKeyLabel = KeyText;

    // Rebind button
    UButton* RebindButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());
    UTextBlock* ButtonText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
    ButtonText->SetText(FText::FromString(TEXT("Rebind")));
    RebindButton->AddChild(ButtonText);
    UHorizontalBoxSlot* ButtonSlot = Row->AddChildToHorizontalBox(RebindButton);
    if (ButtonSlot) ButtonSlot->SetPadding(FMargin(6.0f, 2.0f));
    OutButton = RebindButton;

    return Row;
}

void USettingsMenu::CleanupRebindRows()
{
    // Unbind the row rebind buttons
    for (UButton* Button : RebindButtons)
    {
        if (Button)
        {
            Button->OnClicked.RemoveDynamic(this, &USettingsMenu::OnRebindButtonClicked);
        }
    }
    RebindButtons.Empty();
    RebindKeyLabels.Empty();
}

void USettingsMenu::OnBackClicked()
{
    // If a rebind was in progress, cancel it and clear keyboard focus
    if (bAwaitingRebind)
    {
        bAwaitingRebind = false;
        PendingRebindAction = NAME_None;
        if (TSharedPtr<SWidget> SlateWidget = TakeWidget())
        {
            FSlateApplication::Get().ClearKeyboardFocus(EFocusCause::Cleared);
        }
    }

    // use UIManager to hide settings
    if (UWorld* World = GetWorld())
    {
        if (UGameInstance* GI = World->GetGameInstance())
        {
            if (UUIManager* Manager = Cast<UUIManager>(GI))
            {
                Manager->HideSettings();
            }
        }
    }

    RemoveFromParent();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////// Audio 

void USettingsMenu::OnMasterVolumeChanged(float Value)
{
    SaveAudioToConfig(TEXT("MasterVolume"), Value);
}

void USettingsMenu::OnSFXVolumeChanged(float Value)
{
    SaveAudioToConfig(TEXT("SFXVolume"), Value);
}

void USettingsMenu::OnMusicVolumeChanged(float Value)
{
    SaveAudioToConfig(TEXT("MusicVolume"), Value);
}

void USettingsMenu::SaveAudioToConfig(const FString& Key, float Value)
{
    SaveFloatToConfig(TEXT("Audio"), Key, Value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////// Video 

void USettingsMenu::OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectInfo)
{
    if (SelectedItem.IsEmpty())
    {
        return;
    }

    FString Left, Right;
    if (!SelectedItem.Split(TEXT("x"), &Left, &Right))
    {
        return;
    }

    int32 Width = FCString::Atoi(*Left);
    int32 Height = FCString::Atoi(*Right);

    if (GEngine)
    {
        UGameUserSettings* Settings = GEngine->GetGameUserSettings();
        if (Settings)
        {
            Settings->SetScreenResolution(FIntPoint(Width, Height));
            Settings->ApplySettings(false);
            Settings->SaveSettings();
        }
    }
}

void USettingsMenu::OnWindowModeChanged(FString SelectedItem, ESelectInfo::Type SelectInfo)
{
    if (GEngine == nullptr) return;
    UGameUserSettings* Settings = GEngine->GetGameUserSettings();
    if (Settings == nullptr) return;

    if (SelectedItem == TEXT("Fullscreen"))
    {
        Settings->SetFullscreenMode(EWindowMode::Fullscreen);
    }
    else if (SelectedItem == TEXT("Windowed"))
    {
        Settings->SetFullscreenMode(EWindowMode::Windowed);
    }
    else if (SelectedItem == TEXT("Borderless"))
    {
        Settings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
    }
    Settings->ApplySettings(false);
    Settings->SaveSettings();
}

void USettingsMenu::OnVSyncChanged(bool bIsChecked)
{
    if (GEngine == nullptr) return;
    UGameUserSettings* Settings = GEngine->GetGameUserSettings();
    if (Settings == nullptr) return;

    Settings->SetVSyncEnabled(bIsChecked);
    Settings->ApplySettings(false);
    Settings->SaveSettings();
}

void USettingsMenu::OnBrightnessChanged(float Value)
{
    SaveFloatToConfig(TEXT("Graphics"), TEXT("Brightness"), Value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////// Input rebinding

void USettingsMenu::OnRebindButtonClicked()
{
    // Find which button was clicked
    for (int32 Index = 0; Index < RebindButtons.Num(); ++Index)
    {
        UButton* Btn = RebindButtons[Index];
        if (Btn && Btn->IsPressed())
        {
            const FName ActionName = ActionsToExpose.IsValidIndex(Index) ? ActionsToExpose[Index] : NAME_None;
            if (!ActionName.IsNone())
            {
                StartRebindForAction(ActionName);
            }
            return;
        }
    }
}

FReply USettingsMenu::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    if (bAwaitingRebind && PendingRebindAction != NAME_None)
    {
        const FKey PressedKey = InKeyEvent.GetKey();
        if (PressedKey.IsValid())
        {
            CommitRebind(PendingRebindAction, PressedKey);
            return FReply::Handled();
        }
    }
    return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void USettingsMenu::StartRebindForAction(FName ActionName)
{
    if (ActionName.IsNone()) return;

    bAwaitingRebind = true;
    PendingRebindAction = ActionName;

    // Set keyboard focus to this widget to capture key input
    if (TSharedPtr<SWidget> SlateWidget = TakeWidget())
    {
        FSlateApplication::Get().SetKeyboardFocus(SlateWidget.ToSharedRef(), EFocusCause::SetDirectly);
    }

    // Update the UI label for the matching action to show prompt
    const int32 Index = ActionsToExpose.IndexOfByKey(ActionName);
    if (RebindKeyLabels.IsValidIndex(Index) && RebindKeyLabels[Index])
    {
        RebindKeyLabels[Index]->SetText(FText::FromString(TEXT("Press a key...")));
    }
}

bool USettingsMenu::ApplyRebindToMappingContext(UInputMappingContext* SourceContext, FName ActionName, const FKey& NewKey, int32 PriorityValue)
{
    if (SourceContext == nullptr || !NewKey.IsValid() || ActionName.IsNone())
    {
        return false;
    }

    APlayerController* PC = GetOwningPlayer();
    if (PC == nullptr) return false;
    ULocalPlayer* LP = PC->GetLocalPlayer();
    if (LP == nullptr) return false;

    UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (Subsystem == nullptr) return false;

    // Get or create runtime duplicate of the source context
    UInputMappingContext* RuntimeContext = nullptr;
    if (RuntimeMappingDuplicates.Contains(SourceContext))
    {
        RuntimeContext = RuntimeMappingDuplicates[SourceContext].Get();
    }
    else
    {
        RuntimeContext = DuplicateObject<UInputMappingContext>(SourceContext, this);
        if (RuntimeContext == nullptr) return false;
        RuntimeMappingDuplicates.Add(SourceContext, RuntimeContext);
        Subsystem->AddMappingContext(RuntimeContext, PriorityValue);
    }

    // Find the action mapping and update the key
    for (int32 Index = 0; Index < RuntimeContext->GetMappings().Num(); ++Index)
    {
        FEnhancedActionKeyMapping& Mapping = RuntimeContext->GetMapping(Index);
        if (Mapping.Action && Mapping.Action->GetFName() == ActionName)
        {
            Mapping.Key = NewKey;
            return true;
        }
    }

    return false;
}

// Commit the rebind by saving to config and applying to runtime context
void USettingsMenu::CommitRebind(FName ActionName, const FKey& NewKey)
{
    if (ActionName.IsNone() || !NewKey.IsValid()) return;

    // Save to config
    const FString Section = TEXT("InputBindings");
    if (GConfig)
    {
        GConfig->SetString(*Section, *ActionName.ToString(), *NewKey.ToString(), GGameIni);
        GConfig->Flush(false, GGameIni);
    }

    // Apply to runtime mapping contexts
    for (UInputMappingContext* SrcCtx : ExposedMappingContexts)
    {
        if (SrcCtx == nullptr) continue;
        const TArray<FEnhancedActionKeyMapping>& Mappings = SrcCtx->GetMappings();
        for (const FEnhancedActionKeyMapping& Mapping : Mappings)
        {
            if (Mapping.Action && Mapping.Action->GetFName() == ActionName)
            {
                // Apply rebind
                const int32 PriorityValue = 1;
                if (ApplyRebindToMappingContext(SrcCtx, ActionName, NewKey, PriorityValue))
                {
                    bAwaitingRebind = false;
                    PendingRebindAction = NAME_None;

                    const int32 Index = ActionsToExpose.IndexOfByKey(ActionName);
                    if (RebindKeyLabels.IsValidIndex(Index) && RebindKeyLabels[Index])
                    {
                        RebindKeyLabels[Index]->SetText(FText::FromString(NewKey.GetDisplayName().ToString()));
                    }
                }

                // end focus
                if (TSharedPtr<SWidget> SlateWidget = TakeWidget())
                {
                    FSlateApplication::Get().ClearKeyboardFocus(EFocusCause::Cleared);
                }

                return;
            }
        }
    }

    // If reached, action was not found
    UE_LOG(LogTemp, Warning, TEXT("CommitRebind: action %s not found in any ExposedMappingContexts"), *ActionName.ToString());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////// Helpers
void USettingsMenu::OnResetToDefaultsClicked()
{
    // Clear saved settings to revert to defaults
    if (GConfig)
    {
        const FString InputSection = TEXT("InputBindings");
        for (const FName& Action : ActionsToExpose)
        {
            GConfig->RemoveKey(*InputSection, *Action.ToString(), GGameIni);
        }

        // Clear audio related keys
        const FString AudioSection = TEXT("Audio");
        GConfig->RemoveKey(*AudioSection, TEXT("MasterVolume"), GGameIni);
        GConfig->RemoveKey(*AudioSection, TEXT("SFXVolume"), GGameIni);
        GConfig->RemoveKey(*AudioSection, TEXT("MusicVolume"), GGameIni);

        // Clear video related keys
        const FString GraphicsSection = TEXT("Graphics");
        GConfig->RemoveKey(*GraphicsSection, TEXT("Brightness"), GGameIni);

        // Flush changes to disk
        GConfig->Flush(false, GGameIni);
    }

    // restore default input mappings and runtime contexts
    RestoreDefaultInputMappings();

    // restore default video settings
    if (GEngine)
    {
        UGameUserSettings* Settings = GEngine->GetGameUserSettings();
        if (Settings)
        {
            // Load settings from disk
            Settings->LoadSettings();

            // Apply settings to revert to defaults
            Settings->ApplySettings(false);
            Settings->SaveSettings();
        }
    }

    // Reset audio sliders to default
    if (MasterVolumeSlider) MasterVolumeSlider->SetValue(1.0f);
    if (SFXVolumeSlider) SFXVolumeSlider->SetValue(1.0f);
    if (MusicVolumeSlider) MusicVolumeSlider->SetValue(1.0f);
    if (BrightnessSlider) BrightnessSlider->SetValue(1.0f);

    // Refresh UI to reflect defaults
    LoadSettingsIntoWidgets();
}

void USettingsMenu::RestoreDefaultInputMappings()
{
    APlayerController* PC = GetOwningPlayer();
    if (PC)
    {
        ULocalPlayer* LP = PC->GetLocalPlayer();
        if (LP)
        {
            UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
            if (Subsystem)
            {
                // Remove duplicated runtime contexts
                for (const auto& Pair : RuntimeMappingDuplicates)
                {
                    UInputMappingContext* Context = Pair.Value.Get();
                    if (Context)
                    {
                        Subsystem->RemoveMappingContext(Context);
                    }
                }
                RuntimeMappingDuplicates.Empty();

                // Re-add the exposed mapping contexts if available
                for (UInputMappingContext* Context : ExposedMappingContexts)
                {
                    if (Context)
                    {
                        Subsystem->AddMappingContext(Context, 0);
                    }
                }
            }
        }
    }
}

void USettingsMenu::LoadSettingsIntoWidgets()
{
    // Load audio values from config
    float Value = 1.0f;
    if (GConfig)
    {
        if (GConfig->GetFloat(TEXT("Audio"), TEXT("MasterVolume"), Value, GGameIni) && MasterVolumeSlider)
        {
            MasterVolumeSlider->SetValue(Value);
        }
        Value = 1.0f;
        if (GConfig->GetFloat(TEXT("Audio"), TEXT("SFXVolume"), Value, GGameIni) && SFXVolumeSlider)
        {
            SFXVolumeSlider->SetValue(Value);
        }
        Value = 1.0f;
        if (GConfig->GetFloat(TEXT("Audio"), TEXT("MusicVolume"), Value, GGameIni) && MusicVolumeSlider)
        {
            MusicVolumeSlider->SetValue(Value);
        }
    }

    // Load video settings into widgets
    if (GEngine)
    {
        UGameUserSettings* Settings = GEngine->GetGameUserSettings();
        if (Settings)
        {
            FIntPoint Res = Settings->GetScreenResolution();
            if (ResolutionCombo)
            {
                FString ResString = FString::Printf(TEXT("%dx%d"), Res.X, Res.Y);

                if (ResolutionCombo->FindOptionIndex(ResString) != INDEX_NONE)
                {
                    ResolutionCombo->SetSelectedOption(ResString);
                }
            }

            EWindowMode::Type Mode = Settings->GetFullscreenMode();
            if (WindowModeCombo)
            {
                switch (Mode)
                {
                case EWindowMode::Fullscreen:
                    WindowModeCombo->SetSelectedOption(TEXT("Fullscreen"));
                    break;
                case EWindowMode::Windowed:
                    WindowModeCombo->SetSelectedOption(TEXT("Windowed"));
                    break;
                case EWindowMode::WindowedFullscreen:
                    WindowModeCombo->SetSelectedOption(TEXT("Borderless"));
                    break;
                default:
                    WindowModeCombo->SetSelectedOption(TEXT("Fullscreen"));
                    break;
                }
            }

            if (VSyncCheckBox)
            {
                VSyncCheckBox->SetIsChecked(Settings->IsVSyncEnabled());
            }
        }
    }

    // Brightness load
    float Brightness = 1.0f;
    if (GConfig && GConfig->GetFloat(TEXT("Graphics"), TEXT("Brightness"), Brightness, GGameIni))
    {
        if (BrightnessSlider)
        {
            BrightnessSlider->SetValue(Brightness);
        }
    }

    // Load input bindings from config and apply to runtime mapping contexts
    for (int32 Index = 0; Index < ActionsToExpose.Num(); ++Index)
    {
        const FName ActionName = ActionsToExpose[Index];
        FString BoundKeyString;
        bool bGotFromConfig = false;
        if (GConfig)
        {
            if (GConfig->GetString(TEXT("InputBindings"), *ActionName.ToString(), BoundKeyString, GGameIni))
            {
                bGotFromConfig = true;
            }
        }

        bool bSetLabel = false;

        if (bGotFromConfig && !BoundKeyString.IsEmpty())
        {
            // Try to parse the key from the stored string
            const FName KeyFName(*BoundKeyString);
            FKey NewKey(KeyFName);
            if (!NewKey.IsValid())
            {
                // try full string
                NewKey = FKey(*BoundKeyString);
            }

            if (NewKey.IsValid())
            {
                // Apply to runtime mapping contexts
                for (UInputMappingContext* SrcCtx : ExposedMappingContexts)
                {
                    if (SrcCtx == nullptr) continue;
                    const TArray<FEnhancedActionKeyMapping>& Mappings = SrcCtx->GetMappings();
                    for (const FEnhancedActionKeyMapping& Mapping : Mappings)
                    {
                        if (Mapping.Action && Mapping.Action->GetFName() == ActionName)
                        {
                            // Apply rebind
                            ApplyRebindToMappingContext(SrcCtx, ActionName, NewKey, 1);

                            // Update label to display name
                            if (RebindKeyLabels.IsValidIndex(Index) && RebindKeyLabels[Index])
                            {
                                RebindKeyLabels[Index]->SetText(FText::FromString(NewKey.GetDisplayName().ToString()));
                                bSetLabel = true;
                            }
                            break;
                        }
                    }
                    if (bSetLabel) break;
                }
            }
        }

        if (!bSetLabel)
        {
            // Not found in config, or failed to apply - show current binding from mapping context
            bool bFound = false;
            for (UInputMappingContext* SrcCtx : ExposedMappingContexts)
            {
                if (SrcCtx == nullptr) continue;
                const TArray<FEnhancedActionKeyMapping>& Mappings = SrcCtx->GetMappings();
                for (const FEnhancedActionKeyMapping& Mapping : Mappings)
                {
                    if (Mapping.Action && Mapping.Action->GetFName() == ActionName)
                    {
                        if (RebindKeyLabels.IsValidIndex(Index) && RebindKeyLabels[Index])
                        {
                            RebindKeyLabels[Index]->SetText(FText::FromString(Mapping.Key.GetDisplayName().ToString()));
                        }
                        bFound = true;
                        break;
                    }
                }
                if (bFound) break;
            }

            if (!bFound)
            {
                if (RebindKeyLabels.IsValidIndex(Index) && RebindKeyLabels[Index])
                {
                    RebindKeyLabels[Index]->SetText(FText::FromString(TEXT("Unbound")));
                }
            }
        }
    }
}

void USettingsMenu::SaveFloatToConfig(const FString& Section, const FString& Key, float Value)
{
    if (GConfig)
    {
        // Save to config
        GConfig->SetFloat(*Section, *Key, Value, GGameIni);
        GConfig->Flush(false, GGameIni);
    }
}