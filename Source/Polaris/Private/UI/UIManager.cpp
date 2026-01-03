// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UIManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "GameFramework/HUD.h"
#include "Engine/LocalPlayer.h"
#include "UI/SettingsMenu.h"
#include "UI/CreditsMenu.h"
#include "GameFramework/Pawn.h"

void UUIManager::Init()
{
    Super::Init();

    // Register for map load events
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UUIManager::OnPostLoadMapWithWorld);

    // Create widget instances
    CreateWidgets();

    // if playerController exists, ShowMainMenu
    UWorld* World = GetWorld();
    if (MainMenuWidget)
    {
        if (World && UGameplayStatics::GetPlayerController(World, 0))
        {
            ShowMainMenu();
            return;
        }

        // if playerController hasn't spawned, wait for it to spawn
        if (World)
        {
            bWaitingForPlayerController = true;
            ActorSpawnedHandle = World->AddOnActorSpawnedHandler(FOnActorSpawned::FDelegate::CreateUObject(this, &UUIManager::OnActorSpawned));
            UE_LOG(LogTemp, Verbose, TEXT("UIManager: waiting for PlayerController spawn to show main menu."));
        }
    }
}

void UUIManager::OnPostLoadMapWithWorld(UWorld* LoadedWorld)
{
    if (!LoadedWorld) return;

    const FString MapName = LoadedWorld->GetMapName();
    for (FConstPlayerControllerIterator It = LoadedWorld->GetPlayerControllerIterator(); It; ++It)
    {
        if (APlayerController* PC = It->Get())
        {
            // Restore input mode to game
            PC->bShowMouseCursor = false;
            PC->SetIgnoreMoveInput(false);
            PC->SetIgnoreLookInput(false);
            FInputModeGameOnly GameInputMode;
            PC->SetInputMode(GameInputMode);

            // Enable input on pawn
            if (APawn* Pawn = PC->GetPawn())
            {
                Pawn->EnableInput(PC);
            }

            UE_LOG(LogTemp, Verbose, TEXT("UIManager: Restored game input mode for PlayerController %s"), *GetNameSafe(PC));
        }
    }
    if (MapName.Contains(TEXT("MainMenu")))
    {
        UE_LOG(LogTemp, Verbose, TEXT("UIManager: Detected MainMenu level load — showing main menu."));
        ShowMainMenu();
    }
}

void UUIManager::CreateWidgets()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    if (!MainMenuClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("MainMenuClass is not set in UIManager"));
    }
    else if (!MainMenuWidget)
    {
        MainMenuWidget = CreateWidget<UUserWidget>(World, MainMenuClass);
    }

    if (!SettingsMenuClass)
    {
        UE_LOG(LogTemp, Verbose, TEXT("SettingsMenuClass is not set in UIManager"));
    }
    else if (!SettingsWidget)
    {
        SettingsWidget = CreateWidget<UUserWidget>(World, SettingsMenuClass);
    }

    if (!CreditsMenuClass)
    {
        UE_LOG(LogTemp, Verbose, TEXT("CreditsMenuClass is not set in UIManager"));
    }
    else if (!CreditsWidget)
    {
        CreditsWidget = CreateWidget<UUserWidget>(World, CreditsMenuClass);
    }

    if (!PauseMenuClass)
    {
        UE_LOG(LogTemp, Verbose, TEXT("PauseMenuClass is not set in UIManager"));
    }
    else if (!PauseWidget)
    {
        PauseWidget = CreateWidget<UUserWidget>(World, PauseMenuClass);
    }
}

void UUIManager::HidePlayerUI(APlayerController* PC)
{
    if (!PC)
    {
        UE_LOG(LogTemp, Warning, TEXT("UIManager::HidePlayerUI called with null PlayerController."));
        return;
    }

    // disable HUD rendering
    if (AHUD* HUD = PC->GetHUD())
    {
        UE_LOG(LogTemp, Verbose, TEXT("UIManager: Hiding player HUD."));
        HUD->bShowHUD = false;
    }

    TWeakObjectPtr<APlayerController> WeakPC(PC);
    // delay collapsing widgets to ensure they are fully initialized
    FTickerDelegate TickerDelegate = FTickerDelegate::CreateLambda([this, WeakPC](float DeltaTime) -> bool
        {
            // Only collapse widgets if the game is paused.
            if (!bIsPaused)
            {
                UE_LOG(LogTemp, Verbose, TEXT("UIManager::HidePlayerUI aborting collapse because not paused"));
                return false;
            }

            // Collapse widgets except for menus
            TArray<UUserWidget*> FoundWidgets;
            UObject* WorldContextObject = WeakPC.Get();
            UWidgetBlueprintLibrary::GetAllWidgetsOfClass(WorldContextObject, FoundWidgets, UUserWidget::StaticClass(), false);
            UE_LOG(LogTemp, Verbose, TEXT("UIManager::HidePlayerUI found %d widgets (WorldContextObject=%s)"),
                FoundWidgets.Num(), *GetNameSafe(WorldContextObject));
            for (UUserWidget* Widget : FoundWidgets)
            {
                if (!Widget)
                {
                    continue;
                }

                // Do not collapse menus that should remain visible while paused
                if (Widget == MainMenuWidget ||
                    Widget == PauseWidget ||
                    Widget == SettingsWidget ||
                    Widget == CreditsWidget)
                {
                    continue;
                }

                Widget->SetVisibility(ESlateVisibility::Collapsed);
            }

            // Return false so the ticker removes itself
            return false;
        });

    // single-shot ticker with small delay
    FTSTicker::GetCoreTicker().AddTicker(TickerDelegate, 0.01f);
}

void UUIManager::ShowPlayerUI(APlayerController* PC)
{
    if (!PC)
    {
        UE_LOG(LogTemp, Warning, TEXT("UIManager::ShowPlayerUI called with null PlayerController."));
        return;
    }

    // enable HUD rendering
    if (AHUD* HUD = PC->GetHUD())
    {
        UE_LOG(LogTemp, Verbose, TEXT("UIManager: Showing player HUD."));
        HUD->bShowHUD = true;
    }

    // show widgets except main menu
    TArray<UUserWidget*> FoundWidgets;
    UObject* WorldContextObject = PC;
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(WorldContextObject, FoundWidgets, UUserWidget::StaticClass(), false);
    UE_LOG(LogTemp, Verbose, TEXT("UIManager::ShowPlayerUI found %d widgets (WorldContext=%s)"),
        FoundWidgets.Num(), *GetNameSafe(WorldContextObject));
    for (UUserWidget* Widget : FoundWidgets)
    {
        if (Widget && Widget != MainMenuWidget)
        {
            Widget->SetVisibility(ESlateVisibility::Visible);
        }
    }
}

void UUIManager::ShowMainMenu()
{
    if (!MainMenuWidget)
    {
        CreateWidgets();
        if (!MainMenuWidget)
        {
            return;
        }
    }

    // pause the game while in main menu
    bIsPaused = true;

    UWorld* World = GetWorld();
    APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
    if (PC)
    {
        // If a LocalPlayer is available, make sure the widget is owned by it.
        ULocalPlayer* LP = PC->GetLocalPlayer();
        if (LP)
        {
            if (!MainMenuWidget->GetOwningPlayer() || MainMenuWidget->GetOwningPlayer() != PC)
            {
                // set the owning local player on the existing widget
                MainMenuWidget->SetOwningLocalPlayer(LP);
            }

            // Hide player UI elements
            HidePlayerUI(PC);

            // Set input to UI only and show cursor
            if (!MainMenuWidget->IsInViewport())
            {
                MainMenuWidget->AddToViewport(1000);
                MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
            }

            // set widget to focus
            TSharedPtr<SWidget> SlateWidget = MainMenuWidget->GetCachedWidget();
            FInputModeUIOnly InputMode;
            if (SlateWidget.IsValid() && SlateWidget->SupportsKeyboardFocus())
            {
                InputMode.SetWidgetToFocus(SlateWidget);
            }
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
        }
        else
        {
            UE_LOG(LogTemp, Verbose, TEXT("ShowMainMenu: PlayerController has no LocalPlayer yet."));

            if (!MainMenuWidget->IsInViewport())
            {
                MainMenuWidget->AddToViewport(1000);
                MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
            }

            // attempt to set owning local player once it becomes available.
            if (World)
            {
                World->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &UUIManager::ShowMainMenu));
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ShowMainMenu: no PlayerController available when attempting to set input mode."));
        if (!MainMenuWidget->IsInViewport())
        {
            MainMenuWidget->AddToViewport(1000);
            MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
        }
    }
}

void UUIManager::HideMainMenu()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    if (MainMenuWidget && MainMenuWidget->IsInViewport())
    {
        MainMenuWidget->RemoveFromParent();
    }

    if (PC)
    {
        // Set input to game only and hide cursor
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = false;
    }
}

void UUIManager::OnActorSpawned(AActor* Actor)
{
    if (!bWaitingForPlayerController || !Actor)
    {
        return;
    }

    // If the spawned actor is a PlayerController and it's the local controller, show the menu.
    if (APlayerController* PC = Cast<APlayerController>(Actor))
    {
        if (PC->IsLocalController())
        {
            UWorld* World = Actor->GetWorld();
            if (World)
            {
                World->RemoveOnActorSpawnedHandler(ActorSpawnedHandle);
            }

            bWaitingForPlayerController = false;

            // Ensure widgets exist and show menu
            if (!MainMenuWidget)
            {
                CreateWidgets();
            }

            ShowMainMenu();
        }
    }
}

void UUIManager::ShowSettings()
{
    if (!SettingsWidget)
    {
        CreateWidgets();
        if (!SettingsWidget)
        {
            UE_LOG(LogTemp, Warning, TEXT("ShowSettings: no SettingsWidget class configured."));
            return;
        }
    }

    UWorld* World = GetWorld();
    APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
    if (!PC)
    {
        UE_LOG(LogTemp, Warning, TEXT("ShowSettings: no PlayerController available."));
        return;
    }

    // Ensure widget is owned by local player
    if (ULocalPlayer* LP = PC->GetLocalPlayer())
    {
        if (!SettingsWidget->GetOwningPlayer() || SettingsWidget->GetOwningPlayer() != PC)
        {
            SettingsWidget->SetOwningLocalPlayer(LP);
        }
    }

    // hide pause or main menu
    if (PauseWidget && PauseWidget->IsInViewport())
    {
        PauseWidget->RemoveFromParent();
        bSettingsOpenedFromPause = true;

        // Ensure the main menu isn't left visible underneath settings.
        if (MainMenuWidget && MainMenuWidget->IsInViewport())
        {
            MainMenuWidget->RemoveFromParent();
        }
    }
    else
    {
        HideMainMenu();
        bSettingsOpenedFromPause = false;
        bSettingsOpenedFromMainMenu = true;
    }

    if (!SettingsWidget->IsInViewport())
    {
        SettingsWidget->AddToViewport(1001);
        SettingsWidget->SetVisibility(ESlateVisibility::Visible);
    }

    // Focus the settings widget and show the cursor
    TSharedPtr<SWidget> SlateWidget = SettingsWidget->GetCachedWidget();
    FInputModeUIOnly InputMode;
    if (SlateWidget.IsValid() && SlateWidget->SupportsKeyboardFocus())
    {
        InputMode.SetWidgetToFocus(SlateWidget);
    }
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    PC->SetInputMode(InputMode);
    PC->bShowMouseCursor = true;
}

void UUIManager::HideSettings()
{
    UWorld* World = GetWorld();
    APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);

    if (SettingsWidget && SettingsWidget->IsInViewport())
    {
        SettingsWidget->RemoveFromParent();
    }

    if (!PC)
    {
        return;
    }
 
    // If Settings was opened from the main menu, restore main menu
    if (bSettingsOpenedFromMainMenu)
    {
        bSettingsOpenedFromMainMenu = false;
        ShowMainMenu();
        return;
    }

    // If settings was opened from the pause menu, restore the pause menu first.
    if (bSettingsOpenedFromPause && PauseWidget)
    {
        // Ensure the main menu isn't left visible underneath pause.
        if (MainMenuWidget && MainMenuWidget->IsInViewport())
        {
            MainMenuWidget->RemoveFromParent();
        }

        PauseWidget->AddToViewport(1002);
        PauseWidget->SetVisibility(ESlateVisibility::Visible);
        bSettingsOpenedFromPause = false;

        TSharedPtr<SWidget> SlateWidget = PauseWidget->GetCachedWidget();
        FInputModeUIOnly InputMode;
        if (SlateWidget.IsValid() && SlateWidget->SupportsKeyboardFocus())
        {
            InputMode.SetWidgetToFocus(SlateWidget);
        }
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = true;
        return;
    }

    // If pause is active, return focus to pause menu or keep UI cursor.
    if (bIsPaused || (PauseWidget && PauseWidget->IsInViewport()))
    {
        if (PauseWidget && PauseWidget->IsInViewport())
        {
            TSharedPtr<SWidget> SlateWidget = PauseWidget->GetCachedWidget();
            FInputModeUIOnly InputMode;
            if (SlateWidget.IsValid() && SlateWidget->SupportsKeyboardFocus())
            {
                InputMode.SetWidgetToFocus(SlateWidget);
            }
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
        }
        else
        {
            // If paused but pause widget isn't present, keep cursor visible and use UI-only input.
            PC->bShowMouseCursor = true;
            FInputModeUIOnly InputMode;
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PC->SetInputMode(InputMode);
        }
    }
    else if (MainMenuWidget && MainMenuWidget->IsInViewport())
    {
        // focus main menu if it supports keyboard focus
        TSharedPtr<SWidget> SlateWidget = MainMenuWidget->GetCachedWidget();
        FInputModeUIOnly InputMode;
        if (SlateWidget.IsValid() && SlateWidget->SupportsKeyboardFocus())
        {
            InputMode.SetWidgetToFocus(SlateWidget);
        }
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = true;
    }
    else // restore game input and player UI
    {
        ShowPlayerUI(PC);
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = false;

        // Ensure pawn can receive input
        PC->SetIgnoreMoveInput(false);
        PC->SetIgnoreLookInput(false);
        if (APawn* Pawn = PC->GetPawn())
        {
            Pawn->EnableInput(PC);
        }
    }
}

void UUIManager::ShowCredits()
{
    if (!CreditsWidget)
    {
        CreateWidgets();
        if (!CreditsWidget)
        {
            UE_LOG(LogTemp, Warning, TEXT("ShowCredits: no CreditsWidget class configured."));
            return;
        }
    }

    UWorld* World = GetWorld();
    APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
    if (!PC)
    {
        UE_LOG(LogTemp, Warning, TEXT("ShowCredits: no PlayerController available."));
        return;
    }

    // Ensure widget is owned by local player
    if (ULocalPlayer* LP = PC->GetLocalPlayer())
    {
        if (!CreditsWidget->GetOwningPlayer() || CreditsWidget->GetOwningPlayer() != PC)
        {
            CreditsWidget->SetOwningLocalPlayer(LP);
        }
    }

    // main menu while credits are shown
    HideMainMenu();

    if (!CreditsWidget->IsInViewport())
    {
        CreditsWidget->AddToViewport(1001);
        CreditsWidget->SetVisibility(ESlateVisibility::Visible);
    }

    // Focus credits widget and show cursor if widget supports keyboard focus
    TSharedPtr<SWidget> SlateWidget = CreditsWidget->GetCachedWidget();
    FInputModeUIOnly InputMode;
    if (SlateWidget.IsValid() && SlateWidget->SupportsKeyboardFocus())
    {
        InputMode.SetWidgetToFocus(SlateWidget);
    }
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    PC->SetInputMode(InputMode);
    PC->bShowMouseCursor = true;
}

void UUIManager::HideCredits()
{
    UWorld* World = GetWorld();
    APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);

    if (CreditsWidget && CreditsWidget->IsInViewport())
    {
        CreditsWidget->RemoveFromParent();
    }

    // If pause is active, return focus to pause menu
    if (PC)
    {
        if (bIsPaused || (PauseWidget && PauseWidget->IsInViewport()))
        {
            if (PauseWidget && PauseWidget->IsInViewport())
            {
                TSharedPtr<SWidget> SlateWidget = PauseWidget->GetCachedWidget();
                FInputModeUIOnly InputMode;
                if (SlateWidget.IsValid() && SlateWidget->SupportsKeyboardFocus())
                {
                    InputMode.SetWidgetToFocus(SlateWidget);
                }
                InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                PC->SetInputMode(InputMode);
                PC->bShowMouseCursor = true;
            }
            else
            {
                PC->bShowMouseCursor = true;
                FInputModeUIOnly InputMode;
                InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                PC->SetInputMode(InputMode);
            }
        }
        else if (MainMenuWidget && MainMenuWidget->IsInViewport())
        {
            TSharedPtr<SWidget> SlateWidget = MainMenuWidget->GetCachedWidget();
            FInputModeUIOnly InputMode;
            if (SlateWidget.IsValid() && SlateWidget->SupportsKeyboardFocus())
            {
                InputMode.SetWidgetToFocus(SlateWidget);
            }
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
        }
        else // restore player UI
        {
            ShowPlayerUI(PC);
            FInputModeGameOnly InputMode;
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = false;

            // Ensure pawn can receive input
            PC->SetIgnoreMoveInput(false);
            PC->SetIgnoreLookInput(false);
            if (APawn* Pawn = PC->GetPawn())
            {
                Pawn->EnableInput(PC);
            }
        }
    }
}

void UUIManager::ShowPause()
{
    if (bIsPaused)
    {
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
    if (!PC)
    {
        UE_LOG(LogTemp, Warning, TEXT("ShowPause: no PlayerController available."));
        return;
    }

    // Mark UIManager as paused (don't call SetGamePaused yet to allow timers to run)
    bIsPaused = true;

    // Hide player HUD and other gameplay widgets
    HidePlayerUI(PC);

    // pause the game
    UGameplayStatics::SetGamePaused(World, true);

    if (!PauseWidget)
    {
        CreateWidgets();
        if (!PauseWidget)
        {
            UE_LOG(LogTemp, Warning, TEXT("ShowPause: no PauseWidget class configured."));
            return;
        }
    }

    // Ensure widget is owned by local player
    if (ULocalPlayer* LP = PC->GetLocalPlayer())
    {
        if (!PauseWidget->GetOwningPlayer() || PauseWidget->GetOwningPlayer() != PC)
        {
            PauseWidget->SetOwningLocalPlayer(LP);
        }
    }

    if (!PauseWidget->IsInViewport())
    {
        PauseWidget->AddToViewport(1002);
        PauseWidget->SetVisibility(ESlateVisibility::Visible);
    }

    // Focus pause widget and show cursor
    TSharedPtr<SWidget> SlateWidget = PauseWidget->GetCachedWidget();
    FInputModeUIOnly InputMode;
    if (SlateWidget.IsValid() && SlateWidget->SupportsKeyboardFocus())
    {
        InputMode.SetWidgetToFocus(SlateWidget);
    }
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    PC->SetInputMode(InputMode);
    PC->bShowMouseCursor = true;
}

void UUIManager::HidePause()
{
    if (!bIsPaused)
    {
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
    if (PauseWidget && PauseWidget->IsInViewport())
    {
        PauseWidget->RemoveFromParent();
    }

    // Unpause the game and clear paused state
    UGameplayStatics::SetGamePaused(World, false);
    bIsPaused = false;

    if (PC)
    {
        // Restore input to game and hide cursor
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = false;

        // Restore player UI
        ShowPlayerUI(PC);

        // Ensure pawn can receive input
        PC->SetIgnoreMoveInput(false);
        PC->SetIgnoreLookInput(false);
        if (APawn* Pawn = PC->GetPawn())
        {
            Pawn->EnableInput(PC);
        }
    }
}

void UUIManager::TogglePause()
{
    if (bIsPaused)
    {
        HidePause();
    }
    else
    {
        // do not open pause if main menu is open
        if (MainMenuWidget && MainMenuWidget->IsInViewport())
        {
            UE_LOG(LogTemp, Verbose, TEXT("TogglePause: Main menu open, don't pause"));
            return;
        }
        ShowPause();
    }
}

void UUIManager::SetPauseState(bool bPause)
{
    UWorld* World = GetWorld();
    if (bPause)
    {
        UGameplayStatics::SetGamePaused(World, true);
        bIsPaused = true;
    }
    else
    {
        UGameplayStatics::SetGamePaused(World, false);
        bIsPaused = false;
    }
}