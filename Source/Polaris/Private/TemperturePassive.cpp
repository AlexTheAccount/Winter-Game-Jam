// Fill out your copyright notice in the Description page of Project Settings.


#include "TemperturePassive.h"

// Sets default values for this component's properties
UTemperturePassive::UTemperturePassive()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UTemperturePassive::CalculateTemp(float _DeltaTime)
{

	float CalcTemp;

	CalcTemp = EnviromentTemp + Insulation;

	FinalTemp = FMath::Lerp<float, float>(FinalTemp, CalcTemp, _DeltaTime * TransitionSpeed);

	Freezing = FinalTemp <= FreezingTickTemp;

	if (Freezing)
	{
		TimeSinceLastFreezingTick += _DeltaTime;
		if (TimeSinceLastFreezingTick >= TimePerFreezingTick)
		{
			OnFreezingTick.Broadcast(FreezingTickDamage);
			TimeSinceLastFreezingTick = 0;
		}
	}

	

}

// Called when the game starts
void UTemperturePassive::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTemperturePassive::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CalculateTemp(DeltaTime);
	// ...
}

