#include "TownHall.h"
#include "RTSGameCharacter.h"
#include <Blueprint/UserWidget.h>
#include <NavigationSystem.h>

ATownHall::ATownHall() : Super()
{
	inventory.capacity = 1000;

	static ConstructorHelpers::FClassFinder<UUserWidget> buttonUI(TEXT("WidgetBlueprint'/Game/TopDownCPP/Blueprints/UI/TownHallButtons_BP.TownHallButtons_BP_C'"));

	if (buttonUI.Class != nullptr)
	{
		unitButtonsTemplate = buttonUI.Class;
	}

	static ConstructorHelpers::FClassFinder<APawn> workerClass(TEXT("/Game/TopDownCPP/Blueprints/NPC"));
	if (workerClass.Class != nullptr)
	{
		workerClassTemplate = workerClass.Class;
	}
}

void ATownHall::CreateWorker()
{
	FActorSpawnParameters spawn;
	spawn.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	FVector loc = UNavigationSystemV1::GetCurrent(GetWorld())->GetRandomPointInNavigableRadius(GetWorld(), GetActorLocation(), 3000);

	ARTSGameCharacter* worker = GetWorld()->SpawnActor<ARTSGameCharacter>(workerClassTemplate, loc, GetActorRotation(), spawn);

	if (worker != NULL) {
		worker->stats.owner = buildingStats.owner;
		worker->stats.team = buildingStats.team;
		worker->stats.characterType = ECharacterType::Worker;
	}
}
