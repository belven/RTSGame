// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTSGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "BaseAI.h"

ARTSGameCharacter::ARTSGameCharacter() : Super()
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
	stats.inventory.capacity = 100;
	stats.gatherAmount = 5;
	stats.unitName = "Character";
	AIControllerClass = ABaseAI::StaticClass();
}

int32 ARTSGameCharacter::GetWeight()
{
	int32 wieght = 0;
	for (FItem item : stats.inventory.items) {
		wieght += item.amount;
	}
	return wieght;
}

int32 ARTSGameCharacter::GetCarryWeight()
{
	return stats.inventory.capacity;
}

int32 ARTSGameCharacter::GetGatherAmount()
{
	return stats.gatherAmount;
}

FCharacterStats& ARTSGameCharacter::GetStats()
{
	return stats;
}

void ARTSGameCharacter::RecieveResources(int32 amount, IResourceInterface* ri)
{
	FItem* itemFound = stats.inventory.FindItem(ri->GetEnumName());

	if (itemFound == NULL) {
		FItem item;
		item.amount = amount;
		item.itemName = ri->GetEnumName();

		stats.inventory.items.Add(item);
	}
	else {
		itemFound->amount += amount;
	}
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
	return stats.team;
}

int32 ARTSGameCharacter::GetOwningPlayer()
{
	return stats.owner;
}