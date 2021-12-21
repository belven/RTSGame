#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "Engine/EngineTypes.h"
#include "AnimalAIController.generated.h"

UCLASS()
class RTSGAME_API AAnimalAIController : public AAIController
{
	GENERATED_BODY()
public:
	AAnimalAIController();
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	void MoveToRandomLocation();
private:
	FTimerHandle idleTimer;
	float idleTime;
	int32 travelDistance;	
};