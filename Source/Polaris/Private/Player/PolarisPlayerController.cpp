// Fill out your copyright notice in the Description page of Project Settings.


#include "Polaris/Public/Player/PolarisPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Polaris/Public/Characters/PolarisFPSCharacter.h"
#include "UI/UIManager.h"

void APolarisPlayerController::BeginPlay()
{
	Super::BeginPlay();


}

void APolarisPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(!IsValid(InputSystem)) return;

	InputSystem->AddMappingContext(MappingContext, 0);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if(!IsValid(EnhancedInputComponent)) return;

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::StopJumping);
    EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &ThisClass::OnPausePressed);
    EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ThisClass::OnInteractPressed);

}

void APolarisPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator YawRotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	GetPawn()->AddMovementInput(ForwardDirection, MovementVector.Y);
	GetPawn()->AddMovementInput(RightDirection, MovementVector.X);
}

void APolarisPlayerController::Look(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	AddYawInput(Axis.X * XSensitivity);
	AddPitchInput(Axis.Y * YSensitivity);
}

void APolarisPlayerController::Jump()
{
	GetCharacter()->Jump();
}

void APolarisPlayerController::StopJumping()
{
	GetCharacter()->StopJumping();
}

void APolarisPlayerController::OnPausePressed()
{
    if (UGameInstance* GI = GetGameInstance())
    {
        if (UUIManager* UIManager = Cast<UUIManager>(GI))
        {
            UIManager->SetPauseState(false);
            UIManager->TogglePause();
        }
    }
}

void APolarisPlayerController::OnInteractPressed()
{
    if (APolarisFPSCharacter* FPSCharacter = Cast<APolarisFPSCharacter>(GetCharacter()))
    {
        //FPSCharacter->Interact();
    }
}