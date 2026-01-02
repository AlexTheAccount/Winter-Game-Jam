// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UIManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/PlayerController.h"

void UUIManager::Init()
{
    Super::Init();

    // Create widget instances
    CreateWidgets();

    // show the main menu immediately
    if (MainMenuWidget)
    {
        ShowMainMenu();
    }
}

void UUIManager::CreateWidgets()
{
    if (!MainMenuClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("MainMenuClass is not set in UIManager"));
        return;
    }

    // Create the main menu widget instance once
    if (!MainMenuWidget)
    {

        UWorld* World = GetWorld();
        if (!World)
        {
            return;
        }

        MainMenuWidget = CreateWidget<UUserWidget>(World, MainMenuClass);
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

    if (!MainMenuWidget->IsInViewport())
    {
        MainMenuWidget->AddToViewport();
    }

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        // Set input to UI only and show cursor
        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = true;
    }
}

void UUIManager::HideMainMenu()
{
    if (MainMenuWidget && MainMenuWidget->IsInViewport())
    {
        MainMenuWidget->RemoveFromParent();
    }

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        // Set input to game only and hide cursor
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = false;
    }
}

void UUIManager::ShowSettings()
{

}

void UUIManager::HideSettings()
{
}

void UUIManager::ShowCredits()
{
}

void UUIManager::HideCredits()
{
}