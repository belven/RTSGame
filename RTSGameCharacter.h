#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Resource.h"
#include "DamagableInterface.h"
#include "ResourceInterface.h"
#include "TeamInterface.h"
#include "RTSGameCharacter.generated.h"

UENUM(BlueprintType)
enum class  ECharacterType : uint8 {
	Worker,
	Military,
	Animal,
	End
};

USTRUCT(BlueprintType)
struct FCharacterStats
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float  currentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float  maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int32  carryWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		FString characterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		ECharacterType characterType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int32  team;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int32  owner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int32  gatherAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		TMap<EResourceType, FResourceStats> resources;
};

UCLASS(Blueprintable)
class ARTSGameCharacter : public ACharacter, public IDamagableInterface, public ITeamInterface
{
	GENERATED_BODY()

public:
	ARTSGameCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		FCharacterStats stats;

	FORCEINLINE ECharacterType GetType() { return stats.characterType; }

	virtual int32 GetWeight();
	virtual int32 GetCarryWeight();
	virtual int32 GetGatherAmount();
	virtual FCharacterStats& GetStats();

	virtual void RecieveResources(int32 amount, IResourceInterface* ri);

	virtual void Tick(float DeltaSeconds) override;
	virtual void TakeDamage(float damage) override;
	virtual float GetHealth() override;
	virtual float GetMaxHealth() override;

	virtual int32 GetTeam() override;
	virtual int32 GetOwningPlayer() override;
};