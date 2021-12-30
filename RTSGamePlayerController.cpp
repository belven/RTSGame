// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTSGamePlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "RTSOverseerer.h"
#include "Engine/World.h"
#include "GameFramework/SpringArmComponent.h"
#include <Components/DecalComponent.h>
#include "DamagableInterface.h"
#include <Components/BoxComponent.h>
#include "DrawDebugHelpers.h"
#include "Animal.h"
#include "RTSGameCharacter.h"
#include "BaseAI.h"
#include "CharacterDetailsUI.h"

ARTSGamePlayerController::ARTSGamePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Hand;

	maxZoom = 4000;
	minZoom = 500;
	zoomRate = 100;

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	selectionArea = CreateDefaultSubobject<UBoxComponent>(TEXT("selectionArea"));
	selectionArea->SetBoxExtent(FVector(0));

	static ConstructorHelpers::FObjectFinder<UMaterial> Gold_Cursor(TEXT("Material'/Game/TopDownCPP/Blueprints/Gold_Cursor.Gold_Cursor'"));
	if (Gold_Cursor.Succeeded())
	{
		materialCursors.Add(EResourceType::Gold, Gold_Cursor.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> Wood_Cursor(TEXT("Material'/Game/TopDownCPP/Blueprints/Wood_Cursor.Wood_Cursor'"));
	if (Wood_Cursor.Succeeded())
	{
		materialCursors.Add(EResourceType::Wood, Wood_Cursor.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> Food_Cursor(TEXT("Material'/Game/TopDownCPP/Blueprints/Food_Cursor.Food_Cursor'"));
	if (Food_Cursor.Succeeded())
	{
		materialCursors.Add(EResourceType::Food, Food_Cursor.Object);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> characterDisplay(TEXT("WidgetBlueprint'/Game/TopDownCPP/Blueprints/CharacterDetails.CharacterDetails_C'"));

	if (characterDisplay.Class != nullptr)
	{
		characterUItemplate = characterDisplay.Class;
	}
}

void ARTSGamePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//CursorToWorld->SetupAttachment(InPawn->GetRootComponent());
}

void ARTSGamePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	GetHitResultUnderCursor(ECC_Visibility, true, hit);

	// keep updating the destination every tick while desired
	if (leftMouseDown)
	{
		//MoveToMouseCursor();
		mouseDownTime += DeltaTime;

		mouseEnd = hit.Location;

		centerMouseLocation = FVector((mouseStart + mouseEnd) / 2);
		dist = FVector::Dist(mouseEnd, mouseStart) / 2;
		selectionSize = FVector(dist, dist, 100);
		DrawDebugBox(GetWorld(), centerMouseLocation, selectionSize, FQuat(0, 0, 0, 0), FColor::Black);
	}
	else {
		mouseDownTime = 0;
	}

	if (CursorToWorld != nullptr)
	{
		FVector CursorFV = hit.ImpactNormal;
		FRotator CursorR = CursorFV.Rotation();
		CursorToWorld->SetWorldLocation(hit.Location);
		CursorToWorld->SetWorldRotation(CursorR);

		if (hit.bBlockingHit)
		{
			AActor* targetFound = hit.GetActor();

			if (targetFound != nullptr && targetFound->Implements<UResourceInterface>())
			{
				IResourceInterface* ri = GetResource(targetFound);
				UMaterial* decal = *materialCursors.Find(ri->GetType());

				if (decal != nullptr)
					CursorToWorld->SetDecalMaterial(decal);
			}
		}
	}
}

void ARTSGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (characterUItemplate != nullptr)
	{
		characterUI = CreateWidget<UCharacterDetailsUI>(this, characterUItemplate);
		characterUI->AddToViewport();
		characterUI->SetVisibility(ESlateVisibility::Visible);

		FCharacterStats cs;
		cs.currentHealth = 123123;
		characterUI->SetStats(cs);
	}
}

void ARTSGamePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ARTSGamePlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ARTSGamePlayerController::OnSetDestinationReleased);

	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &ARTSGamePlayerController::ZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &ARTSGamePlayerController::ZoomOut);

	//InputComponent->BindAction("Left", IE_Pressed, this, &ARTSGamePlayerController::LeftPressed);
	//InputComponent->BindAction("Right", IE_Pressed, this, &ARTSGamePlayerController::RightPressed);
	//InputComponent->BindAction("Up", IE_Pressed, this, &ARTSGamePlayerController:UpPressed);
	//InputComponent->BindAction("Down", IE_Pressed, this, &ARTSGamePlayerController::DownPressed);

	//InputComponent->BindAction("Left", IE_Released, this, &ARTSGamePlayerController::LeftReleased);
	//InputComponent->BindAction("Right", IE_Released, this, &ARTSGamePlayerController::RightReleased);
	//InputComponent->BindAction("Up", IE_Released, this, &ARTSGamePlayerController:UpReleased);
	//InputComponent->BindAction("Down", IE_Released, this, &ARTSGamePlayerController::DownReleased);

	InputComponent->BindAction("RightClick", IE_Pressed, this, &ARTSGamePlayerController::RightClick);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ARTSGamePlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ARTSGamePlayerController::MoveToTouchLocation);
}

void ARTSGamePlayerController::MoveToMouseCursor()
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(Hit.ImpactPoint);
	}
}

void ARTSGamePlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void ARTSGamePlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void ARTSGamePlayerController::OnSetDestinationPressed()
{
	leftMouseDown = true;
	mouseStart = hit.Location;
}

void ARTSGamePlayerController::OnSetDestinationReleased()
{
	leftMouseDown = false;

	if (mouseDownTime > 0.75f) {
		SelectUnits();
	}
}

void ARTSGamePlayerController::AttackTarget(IDamagableInterface* target) {
	if (selectedUnits.Num() > 0) {
		for (ARTSGameCharacter* c : selectedUnits) {
			ABaseAI* con = Cast<ABaseAI>(c->GetController());
			con->AttackTarget(target);
		}
	}
}

void ARTSGamePlayerController::GatherResources(IResourceInterface* res) {
	if (selectedUnits.Num() > 0) {
		for (ARTSGameCharacter* c : selectedUnits) {
			ABaseAI* con = Cast<ABaseAI>(c->GetController());
			con->GatherResource(res);
		}
	}
}

void ARTSGamePlayerController::RightClick()
{
	FHitResult TraceHitResult;
	GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);

	if (TraceHitResult.bBlockingHit)
	{
		AActor* targetFound = TraceHitResult.GetActor();

		bool isDamagable = targetFound->Implements<UDamagableInterface>();
		bool isResource = targetFound->Implements<UResourceInterface>();

		if (isDamagable) {
			IDamagableInterface* damagable = GetDamagable(targetFound);

			FCharacterStats cs;
			cs.currentHealth = damagable->GetHealth();
			characterUI->SetStats(cs);

			if (damagable->GetHealth() > 0) {
				ITeamInterface* team = GetTeam(targetFound);
				int32 targetTeam = team->GetTeam();
				int32 overseererTeam = GetOversereer()->GetTeam();

				if (team->GetOwningPlayer() == -1 || targetTeam != overseererTeam) {
					AttackTarget(damagable);
				}
			}
			else if (isResource) {
				IResourceInterface* res = GetResource(targetFound);
				GatherResources(res);
			}
		}
		else if (isResource) {
			IResourceInterface* res = GetResource(targetFound);
			GatherResources(res);
		}
		else {
			MoveUnits(TraceHitResult.Location);
		}
	}
}

void ARTSGamePlayerController::MoveUnits(FVector loc)
{
	if (selectedUnits.Num() > 0) {
		for (ARTSGameCharacter* c : selectedUnits) {
			ABaseAI* con = Cast<ABaseAI>(c->GetController());
			con->MoveToLocation(loc);
		}
	}
}

void ARTSGamePlayerController::SelectUnits()
{
	selectionArea->SetWorldLocation(centerMouseLocation);
	selectionArea->SetBoxExtent(selectionSize);

	TArray<AActor*> actors;
	selectionArea->GetOverlappingActors(actors);

	if (actors.Num() > 0) {
		selectedUnits.Empty();
		for (AActor* a : actors) {
			if (a->IsA(ARTSGameCharacter::StaticClass())) {
				ARTSGameCharacter* character = Cast<ARTSGameCharacter>(a);

				if (character->GetType() != ECharacterType::Animal)
					selectedUnits.Add(character);
			}
		}
	}
}

IResourceInterface* ARTSGamePlayerController::GetResource(AActor* other)
{
	return  Cast<IResourceInterface>(other);
}

ITeamInterface* ARTSGamePlayerController::GetTeam(AActor* other) {
	return  Cast<ITeamInterface>(other);
}

IDamagableInterface* ARTSGamePlayerController::GetDamagable(AActor* other)
{
	return  Cast<IDamagableInterface>(other);
}

void ARTSGamePlayerController::ZoomIn()
{
	GetOversereer()->GetCameraBoom()->TargetArmLength -= zoomRate;

	if (GetOversereer()->GetCameraBoom()->TargetArmLength < minZoom) {
		GetOversereer()->GetCameraBoom()->TargetArmLength = minZoom;
	}
}

ARTSOverseerer* ARTSGamePlayerController::GetOversereer() {
	if (overseerer == NULL) {
		overseerer = Cast<ARTSOverseerer>(GetPawn());
	}
	return overseerer;
}

void ARTSGamePlayerController::ZoomOut()
{
	GetOversereer()->GetCameraBoom()->TargetArmLength += zoomRate;

	if (GetOversereer()->GetCameraBoom()->TargetArmLength > maxZoom) {
		GetOversereer()->GetCameraBoom()->TargetArmLength = maxZoom;
	}
}