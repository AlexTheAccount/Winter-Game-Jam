// Fill out your copyright notice in the Description page of Project Settings.

#include "Polaris/Public/Characters/PolarisFPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

APolarisFPSCharacter::APolarisFPSCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create first-person camera component
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	
	// Attach camera to the capsule component (head location)
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	
	// Position camera at eye level (adjust Z value for your character's height)
	FirstPersonCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 64.0f));
	
	// Enable camera to rotate with controller
	FirstPersonCamera->bUsePawnControlRotation = true;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...
	bUseControllerRotationYaw = true; // ...but rotates to face where the camera is pointing
}

// Called when the game starts or when spawned
void APolarisFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APolarisFPSCharacter::MoveForward(float AxisValue)
{
	if ((Controller != NULL) && (AxisValue != 0.0f))
	{
		// Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, AxisValue);
	}
}

void APolarisFPSCharacter::MoveRight(float AxisValue)
{
	if ((Controller != NULL) && (AxisValue != 0.0f))
	{
		// Find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Add movement in that direction
		AddMovementInput(Direction, AxisValue);
	}
}

void APolarisFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &APolarisFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APolarisFPSCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

