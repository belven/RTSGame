#include "Building.h"

ABuilding::ABuilding()
{
	PrimaryActorTick.bCanEverTick = true;

	buildingStats.maxHealth = 1000;
	buildingStats.currentHealth = buildingStats.maxHealth;
	buildingStats.unitName = "Building";	
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
	healthChanged.Broadcast(buildingStats.currentHealth);
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
	return buildingStats.team;
}

int32 ABuilding::GetOwningPlayer()
{
	return buildingStats.owner;
}