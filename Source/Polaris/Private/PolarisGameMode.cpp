// Fill out your copyright notice in the Description page of Project Settings.

#include "PolarisGameMode.h"
#include "Polaris/Public/Characters/PolarisFPSCharacter.h"

APolarisGameMode::APolarisGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = APolarisFPSCharacter::StaticClass();
}