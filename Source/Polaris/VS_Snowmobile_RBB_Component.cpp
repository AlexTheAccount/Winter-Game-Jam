// Fill out your copyright notice in the Description page of Project Settings.


#include "VS_Snowmobile_RBB_Component.h"

AVS_Snowmobile_RBB_Component::AVS_Snowmobile_RBB_Component()
{
    PrimaryActorTick.bCanEverTick = false;

    // create and attach the chassis mesh component
    ChassisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChassisMesh"));
    RootComponent = ChassisMesh;

    // set properties for the chassis mesh
    ChassisMesh->SetSimulatePhysics(true);
    ChassisMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    ChassisMesh->SetCollisionObjectType(ECC_PhysicsBody);
    ChassisMesh->SetCollisionResponseToAllChannels(ECR_Block);
    ChassisMesh->SetMobility(EComponentMobility::Movable);
    ChassisMesh->SetCastShadow(true);
    ChassisMesh->bCastDynamicShadow = true;
}
