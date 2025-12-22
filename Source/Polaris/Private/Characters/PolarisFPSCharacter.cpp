// Fill out your copyright notice in the Description page of Project Settings.


#include "Polaris/Public/Characters/PolarisFPSCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


APolarisFPSCharacter::APolarisFPSCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

    //Initialize the Camera Boom
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));

    //Setup Camera Boom attachment to the Root component of the class
    CameraBoom->SetupAttachment(RootComponent);

    //Set the boolean to use the PawnControlRotation to true.
    CameraBoom->bUsePawnControlRotation = true;

    //Initialize the FollowCamera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));

    //Set FollowCamera attachment to the Camera Boom
    FollowCamera->SetupAttachment(CameraBoom);
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
        // find out which direction is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, AxisValue);
    }

}

void APolarisFPSCharacter::MoveRight(float AxisValue)
{
    if ((Controller != NULL) && (AxisValue != 0.0f))
    {
        // find out which direction is right
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get right vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // add movement in that direction
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

