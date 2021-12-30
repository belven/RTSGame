#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TeamInterface.generated.h"

UINTERFACE(MinimalAPI)
class UTeamInterface : public UInterface
{
	GENERATED_BODY()
};

class RTSGAME_API ITeamInterface
{
	GENERATED_BODY()
public:
	virtual int32 GetTeam();
	virtual int32 GetOwningPlayer();
};
