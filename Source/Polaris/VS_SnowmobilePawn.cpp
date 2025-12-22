// Fill out your copyright notice in the Description page of Project Settings.


#include "VS_SnowmobilePawn.h"

// Sets default values
AVS_SnowmobilePawn::AVS_SnowmobilePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // create components
        // RBB component as root
    RBBComponent = CreateDefaultSubobject<AVS_Snowmobile_RBB_Component>(TEXT("RBBComponent"));
    RootComponent = RBBComponent->GetRootComponent();
        // movement component
    MovementComponent = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("MovementComponent"));
    MovementComponent->UpdatedComponent = RBBComponent->GetRootComponent();
        // ski components
    LeftSkiComponent = CreateDefaultSubobject<UVS_SkiPrimitiveComponent>(TEXT("LeftSkiComponent"));
    LeftSkiComponent->SetupAttachment(RBBComponent->GetRootComponent());
    RightSkiComponent = CreateDefaultSubobject<UVS_SkiPrimitiveComponent>(TEXT("RightSkiComponent"));
    RightSkiComponent->SetupAttachment(RBBComponent->GetRootComponent());
        // track component
    TrackComponent = CreateDefaultSubobject<UVS_TrackPrimitiveComponent>(TEXT("TrackComponent"));
    TrackComponent->SetupAttachment(RBBComponent->GetRootComponent());
}

// Called when the game starts or when spawned
void AVS_SnowmobilePawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AVS_SnowmobilePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AVS_SnowmobilePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    // input bindings
    PlayerInputComponent->BindAxis("Accelerate", this, &AVS_SnowmobilePawn::Accelerate);
    PlayerInputComponent->BindAxis("Steer", this, &AVS_SnowmobilePawn::Steer);
    PlayerInputComponent->BindAxis("Brake", this, &AVS_SnowmobilePawn::Brake);
}

void AVS_SnowmobilePawn::Accelerate(float Value)
{
    AddMovementInput(GetActorForwardVector(), Value);
}

void AVS_SnowmobilePawn::Steer(float Value)
{
    AddActorLocalRotation(FRotator(0, Value, 0));
}

void AVS_SnowmobilePawn::Brake(float Value)
{
    AddMovementInput(GetActorForwardVector(), -Value);
}

