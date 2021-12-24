#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamagableInterface.h"
#include "Building.generated.h"

USTRUCT(BlueprintType)
struct FBuildingStats
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float  currentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float  maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		FString buildingName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int32  team;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int32  owner;
};


UCLASS()
class RTSGAME_API ABuilding : public AActor, public IDamagableInterface
{
	GENERATED_BODY()

public:
	ABuilding();

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
	virtual int32 GetOwner() override;
};
