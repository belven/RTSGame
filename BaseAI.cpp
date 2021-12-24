#include "BaseAI.h"
#include "RTSGameCharacter.h"
#include "Resource.h"

ABaseAI::ABaseAI() : Super()
{

}

void ABaseAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (targetActor != nullptr && FVector::Distance(GetCharacter()->GetActorLocation(), targetActor->GetActorLocation()) < 500) {
		switch (currentAction) {
		case EActionType::Attack:
			DamageTarget();
			break;
		case EActionType::Gather:
			Gather();
			break;
		}
	}
}

void ABaseAI::Gather() {
	IResourceInterface* ri = Cast<IResourceInterface>(targetActor);
	if (ri->GetAmount() > 0) {
		ri->TakeResources(1);

		// Check if worker can carry more resources
		// Take resources
		// Add resources to worker
		// Else
		// Move to drop off point
		// Add resources to global supply
		// Remove resources to worker
		// Move back to resource
	}
}

void ABaseAI::DamageTarget() {
	IDamagableInterface* di = Cast<IDamagableInterface>(targetActor);
	if (di->GetHealth() > 0) {
		di->TakeDamage(1);
	} 
	// Check if there are other targets in range
	// Attack next viable target
}

void ABaseAI::AttackTarget(IDamagableInterface* target)
{
	if (target->GetHealth() > 0) {
		currentAction = EActionType::Attack;
		targetActor = Cast<AActor>(target);
		MoveToActor(targetActor);
	}
}

void ABaseAI::GatherResource(IResourceInterface* resource)
{
	if (resource->GetAmount() > 0) {
		currentAction = EActionType::Gather;
		targetActor = Cast<AActor>(resource);
		MoveToActor(targetActor);
	}
}