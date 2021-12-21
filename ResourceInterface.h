#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ResourceInterface.generated.h"

UENUM(BlueprintType)
enum class  EResourceType : uint8 {
	Wood,
	Stone,
	Gold,
	Food
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UResourceInterface : public UInterface
{
	GENERATED_BODY()
};

class RTSGAME_API IResourceInterface
{
	GENERATED_BODY()
public:
	virtual void TakeResources(int32 amount);
	virtual EResourceType GetType();
	virtual int32 GetAmount();
};