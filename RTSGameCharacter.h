#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Resource.h"
#include "DamagableInterface.h"
#include "ResourceInterface.h"
#include "TeamInterface.h"
#include "Enums.h"
#include "RTSGameCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryChange, FInventory, newInventory);

UCLASS(Blueprintable)
class ARTSGameCharacter : public ACharacter, public IDamagableInterface, public ITeamInterface
{
	GENERATED_BODY()

public:
	ARTSGameCharacter();

	FInventoryChange inventoryChange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		FCharacterStats stats;

	FORCEINLINE ECharacterType GetType() { return stats.characterType; }

	virtual int32 GetWeight();
	virtual int32 GetCarryWeight();
	virtual int32 GetGatherAmount();
	virtual FCharacterStats& GetStats();

	void ClearInventory();
	virtual void RecieveResources(int32 amount, IResourceInterface* ri);

	virtual void Tick(float DeltaSeconds) override;
	virtual void TakeDamage(float damage) override;
	virtual float GetHealth() override;
	virtual float GetMaxHealth() override;

	virtual int32 GetTeam() override;
	virtual int32 GetOwningPlayer() override;
};