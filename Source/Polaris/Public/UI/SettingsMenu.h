// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputMappingContext.h"
#include "SettingsMenu.generated.h"

class USlider;
class UComboBoxString;
class UCheckBox;
class UVerticalBox;
class UButton;
class UTextBlock;

UCLASS()
class POLARIS_API USettingsMenu : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

protected:
    // Input Mapping Context to expose for rebinding
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TArray<UInputMappingContext*> ExposedMappingContexts;

    // Audio
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (BindWidget))
    USlider* MasterVolumeSlider = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (BindWidget))
    USlider* SFXVolumeSlider = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (BindWidget))
    USlider* MusicVolumeSlider = nullptr;

    // Video 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (BindWidget))
    UComboBoxString* ResolutionCombo = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (BindWidget))
    UComboBoxString* WindowModeCombo = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (BindWidget))
    UCheckBox* VSyncCheckBox = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (BindWidget))
    USlider* BrightnessSlider = nullptr;

    // Input
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (BindWidget))
    UVerticalBox* InputBindingsContainer = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (BindWidget))
    UButton* ResetDefaultsButton = nullptr;

    // Handlers
    UFUNCTION()
    void OnMasterVolumeChanged(float Value);

    UFUNCTION()
    void OnSFXVolumeChanged(float Value);

    UFUNCTION()
    void OnMusicVolumeChanged(float Value);

    UFUNCTION()
    void OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectInfo);

    UFUNCTION()
    void OnWindowModeChanged(FString SelectedItem, ESelectInfo::Type SelectInfo);

    UFUNCTION()
    void OnVSyncChanged(bool bIsChecked);

    UFUNCTION()
    void OnBrightnessChanged(float Value);

    UFUNCTION()
    void StartRebindForAction(FName ActionName);

    void CommitRebind(FName ActionName, const FKey& NewKey);

    UFUNCTION()
    void OnRebindButtonClicked();

    UFUNCTION()
    void OnResetToDefaultsClicked();

private:
    // Helpers
    bool ApplyRebindToMappingContext(UInputMappingContext* SourceContext, FName ActionName, const FKey& NewKey, int32 PriorityValue = 0);
    void LoadSettingsIntoWidgets();
    void SaveAudioToConfig(const FString& Key, float Value);
    void SaveFloatToConfig(const FString& Section, const FString& Key, float Value);

    // key capture state
    bool bAwaitingRebind = false;
    FName PendingRebindAction = NAME_None;
    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

    // UI elements for input rebinding
    TArray<TObjectPtr<UButton>> RebindButtons;
    TArray<TObjectPtr<UTextBlock>> RebindKeyLabels;
    TMap<TObjectPtr<UInputMappingContext>, TObjectPtr<UInputMappingContext>> RuntimeMappingDuplicates;
};