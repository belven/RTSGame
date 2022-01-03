#pragma once
#include "CoreMinimal.h"
#include "Building.h"
#include "StorageInterface.h"
#include "TownHall.generated.h"

UCLASS()
class RTSGAME_API ATownHall : public ABuilding, public IStorageInterface
{
	GENERATED_BODY()
public:
	ATownHall();
};