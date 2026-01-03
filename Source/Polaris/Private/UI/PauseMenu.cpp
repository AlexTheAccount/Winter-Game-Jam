// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PauseMenu.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/UIManager.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

void UPauseMenu::NativeConstruct()
{
    Super::NativeConstruct();

    // Bind button click events
    if (ResumeButton)
    {
        ResumeButton->OnClicked.AddUniqueDynamic(this, &UPauseMenu::OnResumeClicked);
    }

    if (SettingsButton)
    {
        SettingsButton->OnClicked.AddUniqueDynamic(this, &UPauseMenu::OnSettingsClicked);
    }

    if (QuitButton)
    {
        QuitButton->OnClicked.AddUniqueDynamic(this, &UPauseMenu::OnQuitClicked);
    }
}

void UPauseMenu::OnResumeClicked()
{
    if (UUIManager* UI = Cast<UUIManager>(GetWorld()->GetGameInstance()))
    {
        //UI->SetPauseState(false);
        UI->HidePause();
    }
}

void UPauseMenu::OnSettingsClicked()
{
    if (UUIManager* UI = Cast<UUIManager>(GetWorld()->GetGameInstance()))
    {
        // open settings
        UI->ShowSettings();
    }
}

void UPauseMenu::OnQuitClicked()
{
    // unpause and give control back to UIManager, then load the main menu level.
    UWorld* World = GetWorld();

    // Ensure UI manager clears pause state and hides the pause widget
    if (UUIManager* UI = Cast<UUIManager>(World->GetGameInstance()))
    {
        UI->SetPauseState(false);
        UI->HidePause();
    }

    const FName MainMenuLevelName(TEXT("MainMenu"));
    UE_LOG(LogTemp, Verbose, TEXT("UPauseMenu::OnQuitClicked - opening level %s"), *MainMenuLevelName.ToString());
    UGameplayStatics::OpenLevel(World, MainMenuLevelName);
}