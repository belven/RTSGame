#pragma once
#include "CoreMinimal.h"
#include "Building.h"
#include "StorageInterface.h"
#include "TownHall.generated.h"

class ARTSGameCharacter;

UCLASS()
class RTSGAME_API ATownHall : public ABuilding, public IStorageInterface
{
	GENERATED_BODY()
public:
	ATownHall();

	UFUNCTION(BlueprintCallable, Category = "Townhall")
		void CreateWorker();

private:
	TSubclassOf<ARTSGameCharacter> workerClassTemplate;
};