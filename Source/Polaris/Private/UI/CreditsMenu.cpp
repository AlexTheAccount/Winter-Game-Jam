// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CreditsMenu.h"
#include <UI/UIManager.h>
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UCreditsMenu::NativeConstruct()
{
    Super::NativeConstruct();

    // Bind button click events
    if (BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &UCreditsMenu::OnBackClicked);
    }
}

void UCreditsMenu::NativeDestruct()
{
    // Unbind button click events
    if (BackButton)
    {
        BackButton->OnClicked.RemoveDynamic(this, &UCreditsMenu::OnBackClicked);
    }

    Super::NativeDestruct();
}
void UCreditsMenu::OnBackClicked()
{
    if (UWorld* World = GetWorld())
    {
        if (UGameInstance* GI = World->GetGameInstance())
        {
            if (UUIManager* Manager = Cast<UUIManager>(GI))
            {
                Manager->ShowMainMenu();
            }
        }
    }

    RemoveFromParent();
}

void UCreditsMenu::SetCreditsText(const FText& NewText)
{
    if (CreditsText)
    {
        CreditsText->SetText(NewText);
    }
}