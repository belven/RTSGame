#include "ResourceInterface.h"

FString IResourceInterface::GetEnumName()
{
	return UEnums::EnumToString(TEXT("EResourceType"), static_cast<uint8>(GetType()));
}

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