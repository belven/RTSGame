#include "AnimalAIController.h"
#include "NavigationSystem.h"
#include <GameFramework/Character.h>
#include "Animal.h"
#define mRandomPointInNavigableRadius(start, radius, loc) UNavigationSystemV1::GetCurrent(GetWorld())->GetRandomPointInNavigableRadius(start, radius, loc);

AAnimalAIController::AAnimalAIController()
{
	idleTime = 5.0f;
	travelDistance = 3000.0f;
}

void AAnimalAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AAnimal* c = Cast<AAnimal>(GetCharacter());
	if (c->GetHealth() <= 0) {
		StopMovement();
	}
}

void AAnimalAIController::BeginPlay()
{
	Super::BeginPlay();
	MoveToRandomLocation();
}

void AAnimalAIController::MoveToRandomLocation()
{
	AAnimal* c = Cast<AAnimal>(GetCharacter());

	if (c->GetHealth() > 0) {
		FNavLocation loc;

		mRandomPointInNavigableRadius(c->GetActorLocation(), travelDistance, loc);

		MoveToLocation(loc);

		GetWorld()->GetTimerManager().SetTimer(idleTimer, this, &AAnimalAIController::MoveToRandomLocation, idleTime);
	}
}
