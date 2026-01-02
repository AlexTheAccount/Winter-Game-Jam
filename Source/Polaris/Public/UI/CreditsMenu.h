// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreditsMenu.generated.h"

class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class POLARIS_API UCreditsMenu : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Credits", meta = (BindWidget))
    UTextBlock* CreditsText = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Credits", meta = (BindWidget))
    UButton* BackButton = nullptr;

    UFUNCTION()
    void OnBackClicked();

public:
    void SetCreditsText(const FText& NewText);
};