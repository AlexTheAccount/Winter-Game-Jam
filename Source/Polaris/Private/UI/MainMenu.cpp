// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MainMenu.h"
#include "UI/UIManager.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"

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

    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &UMainMenu::OnExitClicked);
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

    if (ExitButton)
    {
        ExitButton->OnClicked.RemoveDynamic(this, &UMainMenu::OnExitClicked);
    }

    Super::NativeDestruct();
}

void UMainMenu::OnStartClicked()
{
    // If a start level is specified, load that level
    if (StartLevelName != NAME_None)
    {
        UE_LOG(LogTemp, Log, TEXT("Opening level %s"), *StartLevelName.ToString());

        UGameplayStatics::OpenLevel(this, StartLevelName);
    }
    else
        UE_LOG(LogTemp, Warning, TEXT("StartLevelName is not set in UMainMenu::OnStartClicked"));
}

void UMainMenu::OnSettingsClicked()
{
    // show settings
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
    // show credits
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

void UMainMenu::OnExitClicked()
{
    // Hide the main menu
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

    // quit game
    if (APlayerController* PC = GetOwningPlayer())
    {
        UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, true);
    }
    else    
        UE_LOG(LogTemp, Warning, TEXT("PC not found in UMainMenu::OnExitClicked"));

}