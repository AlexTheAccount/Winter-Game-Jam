// Fill out your copyright notice in the Description page of Project Settings.


#include "VS_SkiPrimitiveComponent.h"

UVS_SkiPrimitiveComponent::UVS_SkiPrimitiveComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    // create and attach the ski mesh component
    SkiMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkiMesh"));
    SkiMesh->SetupAttachment(this);

    // set properties for the ski mesh
    SkiMesh->SetSimulatePhysics(true);
    SkiMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SkiMesh->SetCollisionObjectType(ECC_PhysicsBody);
    SkiMesh->SetCollisionResponseToAllChannels(ECR_Block);
    SkiMesh->SetMobility(EComponentMobility::Movable);
    SkiMesh->SetCastShadow(true);
    SkiMesh->bCastDynamicShadow = true;

    // create and attach the ski collision component
    SkiCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SkiCollision"));
    SkiCollision->SetupAttachment(SkiMesh);

    // set properties for the ski collision
    SkiCollision->SetBoxExtent(FVector(50.f, 10.f, 5.f));
    SkiCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SkiCollision->SetCollisionObjectType(ECC_WorldDynamic);
    SkiCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
    SkiCollision->SetMobility(EComponentMobility::Movable);
    SkiCollision->SetGenerateOverlapEvents(true);
    SkiCollision->SetNotifyRigidBodyCollision(true);

    // bind the collision event
    SkiCollision->OnComponentBeginOverlap.AddDynamic(this, &UVS_SkiPrimitiveComponent::OnSkiCollision);
}

void UVS_SkiPrimitiveComponent::OnSkiCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherComp && OtherComp != this)
    {
        UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *OtherActor->GetName());
    }
}
