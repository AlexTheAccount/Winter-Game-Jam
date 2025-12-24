// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodWater.h"
#include "Engine/Engine.h" 
// Sets default values for this component's properties
UFoodWater::UFoodWater()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UFoodWater::CalculateAppetite(float _DeltaTime)
{
	Hunger += _DeltaTime * HungerMult;
	Thirst += _DeltaTime * ThirstMult;

	Hunger = FMath::Clamp(Hunger, 0, MaxHunger);
	Thirst = FMath::Clamp(Thirst, 0, MaxThirst);

	OnAppetite.Broadcast(Hunger, Thirst);


	if (Hunger >= MaxHunger)
	{
		TimeSinceLastStarveTick += _DeltaTime;
	}

	if (TimeSinceLastStarveTick >= TimePerStarveTick)
	{
		OnStarving.Broadcast(StarveTickDamage);
		TimeSinceLastStarveTick = 0;
	}
	

}

void UFoodWater::DecreaseHunger(float _PercentToDecrease)
{
	Hunger -= (MaxHunger * _PercentToDecrease / 100.0f);
	
	Hunger = FMath::Clamp(Hunger, 0, MaxHunger);
	
	OnAppetite.Broadcast(Hunger, Thirst);



}

void UFoodWater::IncreaseHunger(float _PercentToIncrease)
{
	Hunger += (MaxHunger * _PercentToIncrease / 100.0f);

	Hunger = FMath::Clamp(Hunger, 0, MaxHunger);
	OnAppetite.Broadcast(Hunger, Thirst);

}

void UFoodWater::DecreaseThirst(float _PercentToDecrease)
{
	Thirst -= (MaxThirst * _PercentToDecrease / 100.0f);

	Thirst = FMath::Clamp(Thirst, 0, MaxThirst);
	OnAppetite.Broadcast(Hunger, Thirst);


}

void UFoodWater::IncreaseThirst(float _PercentToIncrease)
{
	Thirst += (MaxThirst * _PercentToIncrease / 100.0f);
	
	Thirst = FMath::Clamp(Thirst, 0, MaxThirst);
	OnAppetite.Broadcast(Hunger, Thirst);

}

// Called when the game starts
void UFoodWater::BeginPlay()
{
	Super::BeginPlay();
	Activate();
	// ...
	
}


// Called every frame
void UFoodWater::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CalculateAppetite(DeltaTime);
	// ...
}

