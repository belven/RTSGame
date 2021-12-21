#include "ResourceInterface.h"

void IResourceInterface::TakeResources(int32 amount)
{

}

EResourceType IResourceInterface::GetType()
{
	return EResourceType::Food;
}

int32 IResourceInterface::GetAmount()
{
	return 0;
}