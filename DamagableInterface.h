#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamagableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UDamagableInterface : public UInterface
{
	GENERATED_BODY()
};

class RTSGAME_API IDamagableInterface
{
	GENERATED_BODY()
public:
	virtual void TakeDamage(float damage);
	virtual float GetHealth();
	virtual float GetMaxHealth();
	virtual int32 GetTeam();
	virtual int32 GetOwner();
};
