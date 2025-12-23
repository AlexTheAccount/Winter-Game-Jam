// Fill out your copyright notice in the Description page of Project Settings.


#include "VS_SkiSceneComponent.h"

UVS_SkiSceneComponent::UVS_SkiSceneComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PrePhysics;
}

void UVS_SkiSceneComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!Chassis || !Chassis->ChassisMesh || !SkiMesh)
        return;



    const FVector WorldStart = GetComponentLocation();
    const FVector WorldEnd = WorldStart - FVector(0.f, 0.f, MaxTraceDistance);

    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(SkiTrace), true, GetOwner());
    if (GetWorld()->LineTraceSingleByChannel(Hit, WorldStart, WorldEnd, ECC_Visibility, Params))
        ApplySkiForces(Hit);

    if (bDrawTraceDebug)
    {
        DrawDebugLine(GetWorld(), WorldStart, Hit.ImpactPoint, FColor::Green, false, 0.1f);
        DrawDebugLine(GetWorld(), Hit.ImpactPoint, WorldEnd, FColor::Red, false, 0.1f);
    }
}

void UVS_SkiSceneComponent::ApplySkiForces(const FHitResult& Hit)
{
    // compression (clamped to non-negative)
    float Compression = FMath::Clamp(RestLength - Hit.Distance, 0.0f, RestLength);

    // normal force scalar from spring/damper (chassis velocity projected on normal for damping)
    float RelativeVelocity = FVector::DotProduct(Chassis->ChassisMesh->GetComponentVelocity(), Hit.Normal);
    float NormalScalar = SpringStiffness * Compression - DampingCoefficient * RelativeVelocity;
    NormalScalar = FMath::Max(0.0f, NormalScalar); // normal force cannot pull

    FVector NormalForce = Hit.Normal * NormalScalar;

    // lateral resistance
    FVector LateralVelocity = ComputeLateralVelocity(Hit);
    FVector LateralFriction = FVector::ZeroVector;
    if (!LateralVelocity.IsNearlyZero())
        LateralFriction = -LateralVelocity.GetSafeNormal() * LateralStiffness * Compression;

    // longitudinal friction
        // compute ski forward projected onto contact plane
    FVector SkiForward = SkiMesh->GetForwardVector();
    FVector Normal = Hit.Normal.GetSafeNormal();
    FVector ForwardOnPlane = SkiForward - FVector::DotProduct(SkiForward, Normal) * Normal;
    if (!ForwardOnPlane.IsNearlyZero())
    {
        ForwardOnPlane = ForwardOnPlane.GetSafeNormal();

        // longitudinal speed and velocity vector
        FVector ChassisVel = Chassis->ChassisMesh->GetComponentVelocity();
        float ForwardSpeed = FVector::DotProduct(ChassisVel, ForwardOnPlane);
        FVector LongitudinalVel = ForwardOnPlane * ForwardSpeed;

        // damping proportional to velocity
        const float Dampener = DampenerMultiplier * Compression;
        FVector Force = -LongitudinalVel * Dampener;

        // maximum static/kinetic friction magnitude
        float MaxFriction = SkiFriction * NormalScalar;

        // clamp force so it doesn't exceed friction limit
        if (Force.Size() > MaxFriction)
            Force = Force.GetSafeNormal() * MaxFriction;

        // simple static friction
        if (FMath::Abs(ForwardSpeed) < StaticFrictionThreshold)
        {
            // cancel residual forward motion up to max friction
            float Required = FMath::Clamp(FMath::Abs(ForwardSpeed) * Dampener, 0.0f, MaxFriction);
            Force = -ForwardOnPlane * Required * FMath::Sign(ForwardSpeed);
        }

        // final friction force to apply
        FVector LongitudinalFriction = Force;

        // apply forces at contact point
        Chassis->ChassisMesh->AddForceAtLocation(NormalForce + LateralFriction + LongitudinalFriction, Hit.ImpactPoint);
    }
    else // just apply normal and lateral
        Chassis->ChassisMesh->AddForceAtLocation(NormalForce + LateralFriction, Hit.ImpactPoint);
}

FVector UVS_SkiSceneComponent::ComputeLateralVelocity(const FHitResult& Hit) const
{
    if (!Chassis || !Chassis->ChassisMesh)
        return FVector::ZeroVector;

    // Get the velocity of the chassis mesh at the impact point
    FVector Velocity = Chassis->ChassisMesh->GetComponentVelocity();

    // keep only lateral
    FVector Normal = Hit.Normal.GetSafeNormal();
    FVector LateralVelocity = Velocity - FVector::DotProduct(Velocity, Normal) * Normal;

    return LateralVelocity;
}