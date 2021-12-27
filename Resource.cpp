#include "Resource.h"

AResource::AResource()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AResource::TakeResources(int32 amount)
{
	stats.amount -= amount;
}

EResourceType AResource::GetType()
{
	return stats.resourceType;
}

int32 AResource::GetAmount()
{
	return stats.amount;
}