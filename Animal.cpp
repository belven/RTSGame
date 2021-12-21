#include "Animal.h"

AAnimal::AAnimal()
{
	PrimaryActorTick.bCanEverTick = true;
	resourceStats.amount = 25;
	resourceStats.type = EResourceType::Food;
}

void AAnimal::TakeResources(int32 amount)
{
	resourceStats.amount -= amount;
}

EResourceType AAnimal::GetType()
{
	return resourceStats.type;
}

int32 AAnimal::GetAmount()
{
	return resourceStats.amount;
}