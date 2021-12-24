#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "DamagableInterface.h"
#include "ResourceInterface.h"
#include "BaseAI.generated.h"

class ARTSGameCharacter;
class AResource;

UENUM(BlueprintType)
enum class  EActionType : uint8 {
	Attack,
	Gather,
	Build,
	Move,
	Patrol,
	End
};

UCLASS()
class RTSGAME_API ABaseAI : public AAIController
{
	GENERATED_BODY()
public:
	ABaseAI();
	virtual void Tick(float DeltaTime) override;

	void Gather();
	void DamageTarget();
	void AttackTarget(IDamagableInterface* target);
	void GatherResource(IResourceInterface* resource);

private:
	FTimerHandle ActionRate;
	AActor* targetActor;
	EActionType currentAction;
	
};
