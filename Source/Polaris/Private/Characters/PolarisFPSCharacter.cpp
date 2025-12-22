// Fill out your copyright notice in the Description page of Project Settings.


#include "Polaris/Public/Characters/PolarisFPSCharacter.h"

#include "Camera/CameraComponent.h"
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
}

// Called when the game starts or when spawned
void APolarisFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

