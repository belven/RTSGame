#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Enums.h"
#include "ResourceInterface.generated.h"

UINTERFACE(MinimalAPI)
class UResourceInterface : public UInterface
{
	GENERATED_BODY()
};

class RTSGAME_API IResourceInterface
{
	GENERATED_BODY()
public:
	virtual FString GetEnumName();
	virtual void TakeResources(int32 amount);
	virtual EResourceType GetType();
	virtual int32 GetAmount();
};