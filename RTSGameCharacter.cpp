// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTSGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "BaseAI.h"

ARTSGameCharacter::ARTSGameCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;	   

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	stats.currentHealth = 100;
	stats.maxHealth = 100;

	AIControllerClass = ABaseAI::StaticClass();
}

void ARTSGameCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ARTSGameCharacter::TakeDamage(float damage)
{
	stats.currentHealth -= damage;
}

float ARTSGameCharacter::GetHealth()
{
	return stats.currentHealth;
}

float ARTSGameCharacter::GetMaxHealth()
{
	return stats.maxHealth;
}

int32 ARTSGameCharacter::GetTeam()
{
	return 0;
}

int32 ARTSGameCharacter::GetOwner()
{
	return 0;
}