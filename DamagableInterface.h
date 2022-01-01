#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamagableInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthChanged, float, newHealth);

UINTERFACE(MinimalAPI)
class UDamagableInterface : public UInterface
{
	GENERATED_BODY()
};

class RTSGAME_API IDamagableInterface
{
	GENERATED_BODY()
public:
	FHealthChanged healthChanged;

	virtual void TakeDamage(float damage);
	virtual float GetHealth();
	virtual float GetMaxHealth();
};
