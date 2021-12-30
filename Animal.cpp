#include "Animal.h"

AAnimal::AAnimal() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
	resourceStats.amount = 25;
	resourceStats.resourceType = EResourceType::Food;
	stats.characterType = ECharacterType::Animal;
	stats.owner = -1;
}

void AAnimal::TakeResources(int32 amount)
{
	resourceStats.amount -= amount;
}

EResourceType AAnimal::GetType()
{
	return resourceStats.resourceType;
}

int32 AAnimal::GetAmount()
{
	return resourceStats.amount;
}