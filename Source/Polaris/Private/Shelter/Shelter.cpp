// Fill out your copyright notice in the Description page of Project Settings.

#include "Polaris/Public/Shelter/Shelter.h"
#include "Polaris/Public/Shelter/ShelterDoor.h"
#include "Polaris/Public/Shelter/ShelterFuelComponent.h"
#include "Polaris/Public/Shelter/BedComponent.h"

// Sets default values
AShelter::AShelter()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create components and configure them
    InitializeComponents();
    ConfigureMeshes();
    ConfigureCollisions();
}

// Called when the game starts or when spawned
void AShelter::BeginPlay()
{
    Super::BeginPlay();

    // Bind runtime delegates and set collision state
    BindDelegates();
}

void AShelter::InitializeComponents()
{
    // Root scene component
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    // setup components
    ShelterDoor = CreateDefaultSubobject<UShelterDoor>(TEXT("ShelterDoor"));
    ShelterDoor->SetupAttachment(RootComponent);

    FuelComponent = CreateDefaultSubobject<UShelterFuelComponent>(TEXT("ShelterFuelComponent"));
    FuelComponent->SetupAttachment(RootComponent);

    BedComponent = CreateDefaultSubobject<UBedComponent>(TEXT("BedComponent"));
    BedComponent->SetupAttachment(RootComponent);

    // Meshes
    ShelterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShelterMesh"));
    ShelterMesh->SetupAttachment(RootComponent);

    FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
    FloorMesh->SetupAttachment(RootComponent);

    FuelTankMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FuelTankMesh"));
    FuelTankMesh->SetupAttachment(FuelComponent);

    BedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BedMesh"));
    BedMesh->SetupAttachment(BedComponent);
}

void AShelter::ConfigureMeshes()
{
    if (ShelterMesh)
    {
        ShelterMesh->SetMobility(EComponentMobility::Static);
        ShelterMesh->SetCollisionProfileName(TEXT("BlockAll"));
        ShelterMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        ShelterMesh->SetGenerateOverlapEvents(false);
    }

    if (FloorMesh)
    {
        FloorMesh->SetMobility(EComponentMobility::Static);
        FloorMesh->SetCollisionProfileName(TEXT("BlockAll"));
        FloorMesh->SetGenerateOverlapEvents(false);
    }

    if (FuelTankMesh)
    {
        FuelTankMesh->SetMobility(EComponentMobility::Static);
        FuelTankMesh->SetCollisionProfileName(TEXT("BlockAll"));
        FuelTankMesh->SetGenerateOverlapEvents(false);
    }

    if (BedMesh)
    {
        BedMesh->SetMobility(EComponentMobility::Static);
        BedMesh->SetCollisionProfileName(TEXT("BlockAll"));
        BedMesh->SetGenerateOverlapEvents(false);
    }
}

void AShelter::ConfigureCollisions()
{
    FloorCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("FloorCollision"));
    if (FloorCollision)
    {
        FloorCollision->SetupAttachment(FloorMesh);
        FloorCollision->SetBoxExtent(FVector(200.f, 200.f, 150.f));
        FloorCollision->SetCollisionProfileName(TEXT("BlockAll"));
        FloorCollision->SetGenerateOverlapEvents(false);
    }

    FuelTankCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("FuelTankCollision"));
    if (FuelTankCollision)
    {
        FuelTankCollision->SetupAttachment(FuelTankMesh);
        FuelTankCollision->SetBoxExtent(FVector(50.f, 50.f, 50.f));
        FuelTankCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
        FuelTankCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        FuelTankCollision->SetGenerateOverlapEvents(true);
        FuelTankCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    }

    BedCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BedCollision"));
    if (BedCollision)
    {
        BedCollision->SetupAttachment(BedMesh);
        BedCollision->SetBoxExtent(FVector(50.f, 100.f, 25.f));
        BedCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
        BedCollision->SetGenerateOverlapEvents(true);
    }
}

void AShelter::BindDelegates()
{
    // Setup Fuel Tank collision overlaps
    if (FuelTankCollision && FuelComponent)
    {
        FuelTankCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        FuelTankCollision->SetGenerateOverlapEvents(true);
        FuelTankCollision->OnComponentBeginOverlap.AddDynamic(FuelComponent, &UShelterFuelComponent::OnFuelTankBeginOverlap);
        FuelTankCollision->OnComponentEndOverlap.AddDynamic(FuelComponent, &UShelterFuelComponent::OnFuelTankEndOverlap);
    }
}

void AShelter::OnFuelStateChanged(bool bHasFuel)
{
    if (!ShelterDoor)
    {
        return;
    }

    if (bHasFuel)
    {
        ShelterDoor->OpenDoor();
    }
    else
    {
        ShelterDoor->CloseDoor();
    }
}