#include "Building.h"

ABuilding::ABuilding()
{
	PrimaryActorTick.bCanEverTick = true;

	buildingStats.maxHealth = 1000;
	buildingStats.currentHealth = buildingStats.maxHealth;
}

void ABuilding::BeginPlay()
{
	Super::BeginPlay();	
}

void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABuilding::TakeDamage(float damage)
{
	buildingStats.currentHealth -= damage;
}

float ABuilding::GetHealth()
{
	return buildingStats.currentHealth;
}

float ABuilding::GetMaxHealth()
{
	return buildingStats.maxHealth;
}

int32 ABuilding::GetTeam()
{
	return 0;
}

int32 ABuilding::GetOwner()
{
	return 0;
}