// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UIManager.generated.h"

/**
 * 
 */
UCLASS()
class POLARIS_API UUIManager : public UGameInstance
{
	GENERATED_BODY()

public:
    virtual void Init() override;

    // Show/hide main menu
    void ShowMainMenu();
    void HideMainMenu();
    void ShowSettings();
    void HideSettings();
    void ShowCredits();
    void HideCredits();

protected:
    // Creates widgets referenced by this manager
    void CreateWidgets();

    // main menu widget class
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> MainMenuClass;

    // main menu widget instance
    UPROPERTY()
    UUserWidget* MainMenuWidget = nullptr;
};