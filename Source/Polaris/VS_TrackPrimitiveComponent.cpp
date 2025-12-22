// Fill out your copyright notice in the Description page of Project Settings.


#include "VS_TrackPrimitiveComponent.h"


UVS_TrackPrimitiveComponent::UVS_TrackPrimitiveComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    // create and attach the Track mesh component
    TrackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrackMesh"));
    TrackMesh->SetupAttachment(this);

    // set properties for the Track mesh
    TrackMesh->SetSimulatePhysics(true);
    TrackMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    TrackMesh->SetCollisionObjectType(ECC_PhysicsBody);
    TrackMesh->SetCollisionResponseToAllChannels(ECR_Block);
    TrackMesh->SetMobility(EComponentMobility::Movable);
    TrackMesh->SetCastShadow(true);
    TrackMesh->bCastDynamicShadow = true;

    // create and attach the Track collision component
    TrackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("TrackCollision"));
    TrackCollision->SetupAttachment(TrackMesh);

    // set properties for the Track collision
    TrackCollision->SetBoxExtent(FVector(50.f, 10.f, 5.f));
    TrackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TrackCollision->SetCollisionObjectType(ECC_WorldDynamic);
    TrackCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
    TrackCollision->SetMobility(EComponentMobility::Movable);
    TrackCollision->SetGenerateOverlapEvents(true);
    TrackCollision->SetNotifyRigidBodyCollision(true);

    // bind the collision event
    TrackCollision->OnComponentBeginOverlap.AddDynamic(this, &UVS_TrackPrimitiveComponent::OnTrackCollision);
}

void UVS_TrackPrimitiveComponent::OnTrackCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherComp && OtherComp != this)
    {
        UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *OtherActor->GetName());
    }
}
