#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "DamagableInterface.h"
#include "ResourceInterface.h"
#include "BaseAI.generated.h"

class ARTSGameCharacter;
class AResource;
class UBoxComponent;

UENUM(BlueprintType)
enum class  EActionType : uint8 {
	Attack,
	Gather,
	DepositingResources,
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

	const FString EnumToString(const TCHAR* Enum, int32 EnumValue);
	void DepositeResource();
	void Gather();
	bool FindResource(EResourceType resType, TArray<AActor*> actors);
	void GetNearbyActors(TArray<AActor*>& actors);
	void DamageTarget();
	void AttackTarget(IDamagableInterface* target);
	void GatherResource(IResourceInterface* resource);
	ARTSGameCharacter* GetRTSCharacter();

	AActor* GetTargetActor() const { return targetActor; }
	void SetTargetActor(AActor* val);
	void MoveAI(FVector loc);
	virtual void OnPossess(APawn* InPawn) override;
private:
	FTimerHandle ActionRate;
	AActor* preiviousTarget;
	AActor* targetActor;
	EActionType currentAction;
	UBoxComponent* selectionArea;
	
	float actionDelay;
	FVector bbLocation;
	FVector bbExtent;

	FVector characterBBLocation;
	FVector characterBBExtent;

	ARTSGameCharacter* rtsCharacter;
	   
	bool canPerformActions;
	void CanPerformActions();

	template <class T> static void ShuffleArray(TArray<T>& arrayIn);
};

template <class T>
void ABaseAI::ShuffleArray(TArray<T>& arrayIn)
{
	if (arrayIn.Num() > 0)
	{
		int32 LastIndex = arrayIn.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				arrayIn.Swap(i, Index);
			}
		}
	}
}
