#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ResourceInterface.h"
#include "DamagableInterface.h"
#include "RTSGamePlayerController.generated.h"

class UDecalComponent;
class UBoxComponent;

UCLASS()
class ARTSGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARTSGamePlayerController();
	virtual void OnPossess(APawn* InPawn) override;

protected:
	uint32 leftMouseDown : 1;

	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	void OnResetVR();
	void MoveToMouseCursor();
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	void SetNewMoveDestination(const FVector DestLocation);

	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	void RightClick();
	void SelectUnits();

	IResourceInterface* GetResource(AActor* other);
	IDamagableInterface* GetDamagable(AActor* other);

	void ZoomIn();
	void ZoomOut();
private:
	TMap<EResourceType, UMaterial*> materialCursors;
	UDecalComponent* CursorToWorld;

	int32 maxZoom;
	int32 minZoom;

	float zoomRate;
	float mouseDownTime;
	float dist;

	UBoxComponent* selectionArea;

	FVector mouseStart;
	FVector mouseEnd;
	FVector centerMouseLocation;
	FVector selectionSize;

	FHitResult hit;
};


