// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

class UButton;
class UUIManager;
/**
 * 
 */
UCLASS()
class POLARIS_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

protected:
    // Bind these to Buttons in the UMG designer with the exact names below
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pause", meta = (BindWidget))
    UButton* ResumeButton;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pause", meta = (BindWidget))
    UButton* SettingsButton;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pause", meta = (BindWidget))
    UButton* QuitButton;

    UFUNCTION()
    void OnResumeClicked();

    UFUNCTION()
    void OnSettingsClicked();

    UFUNCTION()
    void OnQuitClicked();
};
