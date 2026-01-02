// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class UButton;
UCLASS()
class POLARIS_API UMainMenu : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

protected:
    // Bound widgets
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainMenu", meta = (BindWidget))
    UButton* StartButton = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainMenu", meta = (BindWidget))
    UButton* SettingsButton = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainMenu", meta = (BindWidget))
    UButton* CreditsButton = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainMenu")
    FName StartLevelName = EName::None;

    // Click handlers
    UFUNCTION()
    void OnStartClicked();

    UFUNCTION()
    void OnSettingsClicked();

    UFUNCTION()
    void OnCreditsClicked();
};