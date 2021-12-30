#include "BaseAI.h"
#include "RTSGameCharacter.h"
#include "Resource.h"
#include "StorageInterface.h"
#include <Kismet/GameplayStatics.h>
#include "ResourceInterface.h"

#define MIN(a,b) (a < b) ? (a) : (b)
#define MAX(a,b) (a > b) ? (a) : (b)

ABaseAI::ABaseAI() : Super()
{
	canPerformActions = true;
	actionDelay = 0.5f;
}

void ABaseAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (canPerformActions && GetTargetActor() != nullptr) {

		if (FVector::Distance(GetCharacter()->GetActorLocation(), GetTargetActor()->GetActorLocation()) < 500) {
			canPerformActions = false;

			switch (currentAction) {
			case EActionType::Attack:
				DamageTarget();
				break;
			case EActionType::Gather:
				Gather();
				break;
			case EActionType::DepositingResources:
				DepositeResource();
				break;
			case EActionType::Build:
				break;
			case EActionType::Move:
				break;
			case EActionType::Patrol:
				break;
			case EActionType::End:
				break;
			default:
				break;
			}

			GetWorld()->GetTimerManager().SetTimer(ActionRate, this, &ABaseAI::CanPerformActions, actionDelay);
		}
		else {
			MoveToActor(GetTargetActor());
		}
	}
}

const FString ABaseAI::EnumToString(const TCHAR* Enum, int32 EnumValue)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, Enum, true);
	if (!EnumPtr)
		return NSLOCTEXT("Invalid", "Invalid", "Invalid").ToString();

#if WITH_EDITOR
	return EnumPtr->GetDisplayNameTextByValue(EnumValue).ToString();
#else
	return EnumPtr->GetEnumName(EnumValue);
#endif
}

void ABaseAI::DepositeResource() {
	if (!GetTargetActor()->Implements<UStorageInterface>()) {
		TArray<AActor*> storageActors;
		UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UStorageInterface::StaticClass(), storageActors);

		for (AActor* store : storageActors) {
			ITeamInterface* team = Cast<ITeamInterface>(store);
			if (team->GetOwningPlayer() == GetRTSCharacter()->GetOwningPlayer()) {
				SetTargetActor(store);
			}
		}
	}
	else {
		IStorageInterface* storage = Cast<IStorageInterface>(GetTargetActor());

		for (TPair<EResourceType, FResourceStats>& res : GetRTSCharacter()->GetStats().resources) {
			FString enumName = EnumToString(TEXT("EResourceType"), static_cast<uint8>(res.Value.resourceType));
			storage->AddItem(enumName, res.Value.amount);
		}

		GetRTSCharacter()->GetStats().resources.Empty();

		if (preiviousTarget->Implements<UResourceInterface>()) {
			SetTargetActor(preiviousTarget);
			currentAction = EActionType::Gather;
		}			
	}
}

void ABaseAI::Gather() {
	IResourceInterface* ri = Cast<IResourceInterface>(GetTargetActor());
	int32 amount = ri->GetAmount();
	int32 space = GetRTSCharacter()->GetCarryWeight() - GetRTSCharacter()->GetWeight();

	// Does the target have resources left
	if (amount > 0) {
		// Do we have carry space left
		if (space > 0) {
			// Find the lowest amount to gather, either the resources has less than we can carry / gather or we lack the space to gather fully
			int32 amountToTake = MIN(amount, space);
			amountToTake = MIN(GetRTSCharacter()->GetGatherAmount(), amountToTake);
			ri->TakeResources(amountToTake);

			GetRTSCharacter()->RecieveResources(amountToTake, ri);
		}
		// We ran out of space so return to storage
		else {
			currentAction = EActionType::DepositingResources;
		}
	}
	// The target ran out of resources, but we still have carry space
	else if (space > 0) {
		TArray<AActor*> resourceActors;
		UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UResourceInterface::StaticClass(), resourceActors);

		if (resourceActors.Num() == 0) {
			currentAction = EActionType::DepositingResources;
		}

		for (AActor* res : resourceActors) {
			IResourceInterface* resi = Cast<IResourceInterface>(res);

			if (resi->GetType() == ri->GetType() && resi->GetAmount() > 0) {
				// Is this resource that can be attacked?
				if (res->Implements<UDamagableInterface>()) {
					IDamagableInterface* damagei = Cast<IDamagableInterface>(res);

					// Does the target have health left?
					if (damagei->GetHealth() > 0) {
						// Attack the target
						SetTargetActor(res);
						currentAction = EActionType::Attack;
						break;
					}
					// They're dead, so gather from it
					else {
						SetTargetActor(res);
						break;
					}
				}
				// They can't be attacked, so gather from it
				else {
					SetTargetActor(res);
					break;
				}
			}
		}
	}
	// We ran out of space so return to storage
	else {
		currentAction = EActionType::DepositingResources;
	}
}

void ABaseAI::DamageTarget() {
	IDamagableInterface* di = Cast<IDamagableInterface>(GetTargetActor());

	if (di->GetHealth() > 0) {
		di->TakeDamage(1);
	}
	else if (GetTargetActor()->Implements<UResourceInterface>()) {
		currentAction = EActionType::Gather;
	}
	// Check if there are other targets in range
	// Attack next viable target
}

void ABaseAI::AttackTarget(IDamagableInterface* target)
{
	if (target->GetHealth() > 0) {
		currentAction = EActionType::Attack;
		SetTargetActor(Cast<AActor>(target));
		MoveToActor(GetTargetActor());
	}
}

void ABaseAI::GatherResource(IResourceInterface* resource)
{
	if (resource->GetAmount() > 0) {
		currentAction = EActionType::Gather;
		SetTargetActor(Cast<AActor>(resource));
		MoveToActor(GetTargetActor());
	}
}

ARTSGameCharacter* ABaseAI::GetRTSCharacter()
{
	if (rtsCharacter == nullptr) {
		rtsCharacter = Cast<ARTSGameCharacter>(GetCharacter());
	}
	return rtsCharacter;
}

void ABaseAI::CanPerformActions()
{
	canPerformActions = true;
}
