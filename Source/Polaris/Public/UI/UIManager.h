// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UIManager.generated.h"

class UUserWidget;
/**
 *
 */
UCLASS()
class POLARIS_API UUIManager : public UGameInstance
{
    GENERATED_BODY()

public:
    virtual void Init() override;

    UFUNCTION()
    void OnPostLoadMapWithWorld(UWorld* LoadedWorld);

    // Show/hide functions
    void ShowMainMenu();
    void HideMainMenu();
    void ShowSettings();
    void HideSettings();
    void ShowCredits();
    void HideCredits();
    void HidePlayerUI(APlayerController* PC);
    void ShowPlayerUI(APlayerController* PC);
    void ShowPause();
    void HidePause();
    void TogglePause();
    void SetPauseState(bool bPause);

protected:
    // Creates widgets referenced by this manager
    void CreateWidgets();

    // main menu widget class
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> MainMenuClass;

    // main menu widget instance
    UPROPERTY()
    UUserWidget* MainMenuWidget = nullptr;

    // settings widget class
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> SettingsMenuClass;

    // settings widget instance
    UPROPERTY()
    UUserWidget* SettingsWidget = nullptr;

    // credits widget class
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> CreditsMenuClass;

    // credits widget instance
    UPROPERTY()
    UUserWidget* CreditsWidget = nullptr;

    // pause widget class
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> PauseMenuClass;

    // pause widget instance
    UPROPERTY()
    UUserWidget* PauseWidget = nullptr;

    void OnActorSpawned(AActor* Actor);

private:
    FDelegateHandle ActorSpawnedHandle;
    bool bWaitingForPlayerController = false;

    bool bIsPaused = false;
    bool bSettingsOpenedFromPause = false;
    bool bSettingsOpenedFromMainMenu = false;
};