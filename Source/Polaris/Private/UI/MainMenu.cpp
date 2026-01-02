// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MainMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"
#include "UI/UIManager.h"
#include "GameFramework/PlayerController.h"

void UMainMenu::NativeConstruct()
{
    Super::NativeConstruct();

    // Bind button click events
    if (StartButton)
    {
        StartButton->OnClicked.AddDynamic(this, &UMainMenu::OnStartClicked);
    }

    if (SettingsButton)
    {
        SettingsButton->OnClicked.AddDynamic(this, &UMainMenu::OnSettingsClicked);
    }

    if (CreditsButton)
    {
        CreditsButton->OnClicked.AddDynamic(this, &UMainMenu::OnCreditsClicked);
    }
}

void UMainMenu::NativeDestruct()
{
    // Unbind button click events
    if (StartButton)
    {
        StartButton->OnClicked.RemoveDynamic(this, &UMainMenu::OnStartClicked);
    }

    if (SettingsButton)
    {
        SettingsButton->OnClicked.RemoveDynamic(this, &UMainMenu::OnSettingsClicked);
    }

    if (CreditsButton)
    {
        CreditsButton->OnClicked.RemoveDynamic(this, &UMainMenu::OnCreditsClicked);
    }

    Super::NativeDestruct();
}

void UMainMenu::OnStartClicked()
{
    // If a start level is specified, load that level
    if (StartLevelName != NAME_None)
    {
        UGameplayStatics::OpenLevel(this, StartLevelName);
        return;
    }

    // Otherwise, hide the main menu via UIManager
    if (UWorld* World = GetWorld())
    {
        if (UGameInstance* GI = World->GetGameInstance())
        {
            if (UUIManager* Manager = Cast<UUIManager>(GI))
            {
                Manager->HideMainMenu();
            }
        }
    }

    // Set input mode back to game only and hide mouse cursor
    if (APlayerController* PC = GetOwningPlayer())
    {
        PC->bShowMouseCursor = false;
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
    }
}

void UMainMenu::OnSettingsClicked()
{
    if (UWorld* World = GetWorld())
    {
        if (UGameInstance* GI = World->GetGameInstance())
        {
            if (UUIManager* Manager = Cast<UUIManager>(GI))
            {
                Manager->ShowSettings();
            }
        }
    }
}

void UMainMenu::OnCreditsClicked()
{
    if (UWorld* World = GetWorld())
    {
        if (UGameInstance* GI = World->GetGameInstance())
        {
            if (UUIManager* Manager = Cast<UUIManager>(GI))
            {
                Manager->ShowCredits();
            }
        }
    }
}