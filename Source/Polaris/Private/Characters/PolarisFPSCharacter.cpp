// Fill out your copyright notice in the Description page of Project Settings.


#include "Polaris/Public/Characters/PolarisFPSCharacter.h"
#include "UObject/NoExportTypes.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


APolarisFPSCharacter::APolarisFPSCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 0.0f;
	CameraBoom->SetRelativeLocation(FVector(10.f, 0.f, 90.f));

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraBoom);
	Camera->bUsePawnControlRotation = true;
}

void APolarisFPSCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FHitResult FloorHit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.bReturnPhysicalMaterial = true;
	
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorUpVector() * -500.f;
	GetWorld()->LineTraceSingleByChannel(FloorHit, Start, End, ECC_Visibility, Params);
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false);
	
	if (FloorHit.bBlockingHit)
	{
		if (FloorHit.PhysMaterial.IsValid() && FloorHit.PhysMaterial->SurfaceType == SurfaceType1)
		{
			GetCharacterMovement()->BrakingDecelerationWalking = 350.f;
			GetCharacterMovement()->GroundFriction = 0.f;
		}
		else
		{
			GetCharacterMovement()->BrakingDecelerationWalking = 2048.f;
			GetCharacterMovement()->GroundFriction = 8.f;
			
		}
	}
}
