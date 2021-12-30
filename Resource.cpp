#include "Resource.h"

AResource::AResource()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AResource::TakeResources(int32 amount)
{
	stats.amount -= amount;

	if (stats.amount <= 0) {
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
}

EResourceType AResource::GetType()
{
	return stats.resourceType;
}

int32 AResource::GetAmount()
{
	return stats.amount;
}