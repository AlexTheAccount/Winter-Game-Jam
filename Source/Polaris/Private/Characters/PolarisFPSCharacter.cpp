// Fill out your copyright notice in the Description page of Project Settings.


#include "Polaris/Public/Characters/PolarisFPSCharacter.h"

#include "Blueprint/UserWidget.h"
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

void APolarisFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (QTEClass)
	{
		QTEWidget = CreateWidget<UUserWidget>(Cast<APlayerController>(GetController()), QTEClass);
	}


	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("FallProgress"));
		FallTimeline.AddInterpFloat(CurveFloat, TimelineProgress, FName("Value"));

		FOnTimelineEventStatic OnFallFinish;
		OnFallFinish.BindUFunction(this, FName("FallFinished"));
		FallTimeline.SetTimelineFinishedFunc(OnFallFinish);
		FallTimeline.SetTimelineLength(1.f);
	}
}

void APolarisFPSCharacter::FallProgress(float Value)
{
	FVector NewLocation = FMath::Lerp(CameraStartLoc, CameraEndLoc, Value);
	FRotator NewRotation = FMath::RInterpTo(CameraStartRot, CameraEndRot, Value, 1.f);
	Camera->SetRelativeLocation(NewLocation);
	Controller->SetControlRotation(NewRotation);
}

void APolarisFPSCharacter::FallFinished()
{
	auto FinishedFunc = &ThisClass::FinishedGettingUp;
	float Delay = 0.01f;
	if (!bIsGettingUp)
	{
		bIsGettingUp = true;
		FinishedFunc = &ThisClass::GetUp;
		Delay = 2.f;
	}
	GetWorldTimerManager().SetTimer(GetUpTimer, this, FinishedFunc, Delay);
}

void APolarisFPSCharacter::GetUp()
{
	if (bIsGettingUp)
	{
		FallTimeline.ReverseFromEnd();
		bIsFalling = false;
	}
}

void APolarisFPSCharacter::FinishedGettingUp()
{
	bIsGettingUp = false;
	FallTimeline.Stop();
	Controller->EnableInput(Cast<APlayerController>(Controller));
	GetMesh()->SetVisibility(true);
}

void APolarisFPSCharacter::TraceForFloor(FHitResult& FloorHit)
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.bReturnPhysicalMaterial = true;

	FVector Start = GetActorLocation();
	FVector End = Start + GetActorUpVector() * -500.f;
	GetWorld()->LineTraceSingleByChannel(FloorHit, Start, End, ECC_Visibility, Params);
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false);
}

void APolarisFPSCharacter::StartFall()
{
	bIsFalling = true;
	GetCharacterMovement()->StopMovementImmediately();
	Controller->DisableInput(Cast<APlayerController>(Controller));

	CameraStartRot = Controller->GetControlRotation();
	CameraEndRot = CameraStartRot + FRotator(90.f, 0.f, -90.f);

	CameraStartLoc = Camera->GetRelativeLocation();
	CameraEndLoc = CameraStartLoc + FVector(0.f, 0.f, -100.f);

	GetMesh()->SetVisibility(false);
	FallTimeline.PlayFromStart();
}

void APolarisFPSCharacter::StartQTE()
{
	bQTEActive = true;
	QTEWidget->AddToViewport();
	GetWorldTimerManager().SetTimer(QTETimer, this, &ThisClass::EndQTE, QTEEndTime);
}

void APolarisFPSCharacter::EndQTE()
{
	if (!bQTESuccess)
	{
		StartFall();
	}
	ResetQTEValues();
}


void APolarisFPSCharacter::QTESuccessed()
{
	GetWorldTimerManager().ClearTimer(QTETimer);
	ResetQTEValues();
}

void APolarisFPSCharacter::ResetQTEValues()
{
	bQTESuccess = false;
	bQTEActive = false;
	SlipAmount = 0.f;
	QTEWidget->RemoveFromParent();
}

void APolarisFPSCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FallTimeline.TickTimeline(DeltaSeconds);

	FHitResult FloorHit;
	TraceForFloor(FloorHit);

	if (FloorHit.bBlockingHit)
	{
		if (FloorHit.PhysMaterial.IsValid() && FloorHit.PhysMaterial->SurfaceType == SurfaceType1)
		{
			GetCharacterMovement()->BrakingDecelerationWalking = 350.f;
			GetCharacterMovement()->GroundFriction = 0.f;

			if (GetCharacterMovement()->Velocity.Size() >= 400)
			{
				SlipAmount += 0.1f;
				if (SlipAmount >= SlipThreshold)
				{
					if (!bQTEActive)
					{
						StartQTE();
					}
				}
			}
			else
			{
				SlipAmount -= 0.1f;
				if (SlipAmount <= 0)
				{
					SlipAmount = 0.f;
				}
			}
			GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Green,
			                                 FString::Printf(
				                                 TEXT("Amount: %f Threshold: %f"), SlipAmount, SlipThreshold));
		}
		else
		{
			SlipAmount = 0.f;
			GetCharacterMovement()->BrakingDecelerationWalking = 2048.f;
			GetCharacterMovement()->GroundFriction = 8.f;
		}
	}
}
