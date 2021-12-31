#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamagableInterface.h"
#include "TeamInterface.h"
#include "Enums.h"
#include "Building.generated.h"

UCLASS()
class RTSGAME_API ABuilding : public AActor, public IDamagableInterface, public ITeamInterface
{
	GENERATED_BODY()

public:
	ABuilding();

	FBuildingStats GetBuildingStats() const { return buildingStats; }
	void SetBuildingStats(FBuildingStats val) { buildingStats = val; }
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		FBuildingStats buildingStats;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void TakeDamage(float damage) override;
	virtual float GetHealth() override;
	virtual float GetMaxHealth() override;
	virtual int32 GetTeam() override;
	virtual int32 GetOwningPlayer() override;
};
