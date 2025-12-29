// Fill out your copyright notice in the Description page of Project Settings.


#include "Polaris/Public/Snowmobile/SkiSceneComponent.h"
#include "Polaris/Public/Snowmobile/ChassisComponent.h"

USkiSceneComponent::USkiSceneComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PrePhysics;
}

void USkiSceneComponent::BeginPlay()
{
    // perform a single trace down to measure the starting rest distance
    const FVector Up = GetComponentTransform().GetUnitAxis(EAxis::Z);
    const FVector WorldStart = GetComponentLocation() + Up * 1.0f;
    const FVector WorldEnd = WorldStart - Up * MaxTraceDistance;

    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(SkiTrace_BeginPlay), true, GetOwner());
    Params.AddIgnoredComponent(SkiMesh);

    if (GetWorld()->LineTraceSingleByChannel(Hit, WorldStart, WorldEnd, ECC_Visibility, Params))
    {
        const float DistanceAlongUp = FVector::DotProduct(GetComponentLocation() - Hit.ImpactPoint, Up);
        RestLength = FMath::Clamp(DistanceAlongUp, 0.0f, MaxTraceDistance);

        DrawDebugLine(GetWorld(), WorldStart, Hit.ImpactPoint, FColor::Green, false, 5.0f);
        DrawDebugLine(GetWorld(), Hit.ImpactPoint, WorldEnd, FColor::Red, false, 5.0f);
    }
}

void USkiSceneComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!Chassis || !SkiMesh)
        return;
    
    const FVector Up = GetComponentTransform().GetUnitAxis(EAxis::Z);
    const FVector WorldStart = GetComponentLocation() + Up * 1.0f;
    const FVector WorldEnd = WorldStart - Up * MaxTraceDistance;

    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(SkiTrace), true, GetOwner());
    Params.AddIgnoredComponent(SkiMesh);
    if (GetWorld()->LineTraceSingleByChannel(Hit, WorldStart, WorldEnd, ECC_Visibility, Params))
    {
        if (Hit.GetComponent() && Hit.GetComponent() != SkiMesh)
        {
            ApplySkiForces(Hit);

            if (bDrawTraceDebug)
            {
                DrawDebugLine(GetWorld(), WorldStart, Hit.ImpactPoint, FColor::Green, false, 0.1f);
                DrawDebugLine(GetWorld(), Hit.ImpactPoint, WorldEnd, FColor::Red, false, 0.1f);
            }
        }
    }
}

void USkiSceneComponent::ApplySkiForces(const FHitResult& Hit)
{
    if (!Chassis || !SkiMesh)
        return;

    // compute compression based on rest length and ski up axis
    const FVector Up = GetComponentTransform().GetUnitAxis(EAxis::Z);
    const FVector AnchorPosition = GetComponentLocation();
    const float DistanceAlongUp = FVector::DotProduct(AnchorPosition - Hit.ImpactPoint, Up);
    const float CompressionScaled = FMath::Clamp(RestLength - DistanceAlongUp, 0.0f, RestLength);
    const float Compression = CompressionScaled * LengthScale;

    // Compute relative velocity at contact point
    FVector ChassisVelocityAtPoint = Chassis->GetPhysicsLinearVelocityAtPoint(Hit.ImpactPoint);
    FVector SurfaceVelocityAtPoint = FVector::ZeroVector;
    if (UPrimitiveComponent* HitComponent = Hit.GetComponent())
    {
        // If hit component is simulating physics, get its velocity at the point
        if (HitComponent->IsSimulatingPhysics())
        {
            SurfaceVelocityAtPoint = HitComponent->GetPhysicsLinearVelocityAtPoint(Hit.ImpactPoint);
        }
    }
    const FVector RelativeVelocityScaled = ChassisVelocityAtPoint - SurfaceVelocityAtPoint;
    const FVector RelativeVelocity = RelativeVelocityScaled * LengthScale;

    // normal force calculation
    const FVector Normal = Hit.Normal.GetSafeNormal();
    const float RelativeVelocityAlongNormal = FVector::DotProduct(RelativeVelocity, Normal);

    // spring + damping along normal
    float NormalScalar = SpringStiffness * Compression - DampingCoefficient * RelativeVelocityAlongNormal;
    NormalScalar = FMath::Max(0.0f, NormalScalar);
    const FVector NormalForce = Normal * NormalScalar;

    // lateral (side) friction/damping
    FVector LateralVelocityScaled = ComputeLateralVelocity(Hit);
    FVector LateralVelocity = LateralVelocityScaled * LengthScale;

    // remove surface lateral velocity if any
    if (!SurfaceVelocityAtPoint.IsNearlyZero())
    {
        FVector SurfaceLateral = SurfaceVelocityAtPoint;
        SurfaceLateral -= FVector::DotProduct(SurfaceLateral, Normal) * Normal;
        SurfaceLateral *= LengthScale;

        // remove forward component
        FVector SkiForward = SkiMesh->GetForwardVector();
        FVector ForwardOnPlane = SkiForward - FVector::DotProduct(SkiForward, Normal) * Normal;
        if (!ForwardOnPlane.IsNearlyZero())
        {
            ForwardOnPlane.Normalize();
            SurfaceLateral -= FVector::DotProduct(SurfaceLateral, ForwardOnPlane) * ForwardOnPlane;
        }

        // remove surface lateral from lateral velocity
        LateralVelocity -= SurfaceLateral;
    }
    
    // lateral damping proportional to lateral relative velocity and compression
    FVector LateralFriction = FVector::ZeroVector;
    if (!LateralVelocity.IsNearlyZero())
    {
        // apply lateral damping
        LateralFriction = -LateralVelocity * LateralStiffness * Compression;
    }

    // longitudinal (forward) friction/damping
    FVector SkiForward = SkiMesh->GetForwardVector();
    FVector ForwardOnPlane = SkiForward - FVector::DotProduct(SkiForward, Normal) * Normal;
    if (!ForwardOnPlane.IsNearlyZero())
    {
        ForwardOnPlane = ForwardOnPlane.GetSafeNormal();

        const float ForwardSpeed = FVector::DotProduct(RelativeVelocity, ForwardOnPlane);
        const FVector LongitudinalVelocity = ForwardOnPlane * ForwardSpeed;

        // viscous damping proportional to compression
        const float Dampener = DampenerMultiplier * Compression;
        FVector LongitudinalForce = -LongitudinalVelocity * Dampener;

        // maximum friction based on normal force
        float MaxFriction = SkiFriction * NormalScalar;

        // clamp to max friction
        if (LongitudinalForce.Size() > MaxFriction)
            LongitudinalForce = LongitudinalForce.GetSafeNormal() * MaxFriction;

        // static friction to prevent creeping when nearly stopped
        const float StaticThresholdScaled = StaticFrictionThreshold * LengthScale;
        if (FMath::Abs(ForwardSpeed) < StaticThresholdScaled)
        {
            // compute required force to stop movement
            float Required = FMath::Clamp(FMath::Abs(ForwardSpeed) * Dampener, 0.0f, MaxFriction);

            // override longitudinal force to prevent movement
            LongitudinalForce = -ForwardOnPlane * Required * FMath::Sign(ForwardSpeed);
        }

        // apply longitudinal force
        Chassis->AddForceAtLocation(NormalForce + LateralFriction + LongitudinalForce, Hit.ImpactPoint);
    }
    else // apply normal + lateral only if no forward direction
        Chassis->AddForceAtLocation(NormalForce + LateralFriction, Hit.ImpactPoint);
}

FVector USkiSceneComponent::ComputeLateralVelocity(const FHitResult& Hit) const
{
    if (!Chassis || !SkiMesh)
        return FVector::ZeroVector;

    // get chassis velocity at contact point
    FVector Velocity = Chassis->GetPhysicsLinearVelocityAtPoint(Hit.ImpactPoint);

    // remove normal
    FVector Normal = Hit.Normal.GetSafeNormal();
    Velocity -= FVector::DotProduct(Velocity, Normal) * Normal;

    // remove forward to leave lateral
    FVector SkiForward = SkiMesh->GetForwardVector();
    FVector ForwardOnPlane = SkiForward - FVector::DotProduct(SkiForward, Normal) * Normal;
    if (!ForwardOnPlane.IsNearlyZero())
    {
        ForwardOnPlane = ForwardOnPlane.GetSafeNormal();
        Velocity -= FVector::DotProduct(Velocity, ForwardOnPlane) * ForwardOnPlane;
    }

    return Velocity;
}