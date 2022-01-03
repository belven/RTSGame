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
#include "Building.h"
#include "StorageInterface.h"

#define mTraceChannel ECollisionChannel::ECC_Pawn

ARTSGamePlayerController::ARTSGamePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Hand;

	maxZoom = 4000;
	minZoom = 500;
	zoomRate = 100;
	MoveSpeed = 800.0f;

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

	static ConstructorHelpers::FClassFinder<UUserWidget> characterDisplay(TEXT("WidgetBlueprint'/Game/TopDownCPP/Blueprints/UI/CharacterDetails.CharacterDetails_C'"));

	if (characterDisplay.Class != nullptr)
	{
		characterUItemplate = characterDisplay.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> inventory(TEXT("WidgetBlueprint'/Game/TopDownCPP/Blueprints/UI/InventoryUI_BP.InventoryUI_BP_C'"));

	if (inventory.Class != nullptr)
	{
		inventoryTemplate = inventory.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> contextUI(TEXT("WidgetBlueprint'/Game/TopDownCPP/Blueprints/UI/ContextUI_BP.ContextUI_BP_C'"));

	if (contextUI.Class != nullptr)
	{
		contextTemplate = contextUI.Class;
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

	GetHitResultUnderCursor(mTraceChannel, true, hit);
	CalculateMovement(DeltaTime);

	// keep updating the destination every tick while desired
	if (leftMouseDown)
	{
		//MoveToMouseCursor();
		mouseDownTime += DeltaTime;

		mouseEnd = hit.Location;

		centerMouseLocation = FVector((mouseStart + mouseEnd) / 2);
		dist = FVector::Dist(mouseEnd, mouseStart) / 2;
		selectionSize = FVector(dist, dist, 100);

		if (mouseDownTime > 0.75f)
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
		characterUI->SetVisibility(ESlateVisibility::Hidden);
	}

	if (inventoryTemplate != nullptr)
	{
		inventoryUI = CreateWidget<UInventoryUI>(this, inventoryTemplate);
		inventoryUI->SetVisibility(ESlateVisibility::Hidden);
	}

	if (contextTemplate != nullptr)
	{
		contextUnitUI = CreateWidget<UContextUnitUI>(this, contextTemplate);
		contextUnitUI->SetVisibility(ESlateVisibility::Visible);
		contextUnitUI->GenerateUI(characterUI, inventoryUI);
		contextUnitUI->AddToViewport();
	}
}

const FName ARTSGamePlayerController::MoveForwardBinding("MoveForward");
const FName ARTSGamePlayerController::MoveRightBinding("MoveRight");

void ARTSGamePlayerController::CalculateMovement(float DeltaSeconds)
{
	// Find movement direction
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding); // W S
	const float RightValue = GetInputAxisValue(MoveRightBinding); // A D

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

	// Calculate  movement
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{
		GetCharacter()->AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X), ForwardValue);
		GetCharacter()->AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y), RightValue);
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
	
	InputComponent->BindAction("RightClick", IE_Pressed, this, &ARTSGamePlayerController::RightClick);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ARTSGamePlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ARTSGamePlayerController::MoveToTouchLocation);

	// set up game play key bindings
	InputComponent->BindAxis(MoveForwardBinding);
	InputComponent->BindAxis(MoveRightBinding);
}

void ARTSGamePlayerController::MoveToMouseCursor()
{
	if (hit.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(hit.ImpactPoint);
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
	SelectUnits();
}

void ARTSGamePlayerController::AttackTarget(IDamagableInterface* target) {
	if (selectedUnits.Num() > 0) {
		for (AActor* a : selectedUnits) {
			if (a->IsA(ARTSGameCharacter::StaticClass())) {
				ARTSGameCharacter* c = Cast<ARTSGameCharacter>(a);

				if (c->GetOwningPlayer() != -1) {
					ABaseAI* con = Cast<ABaseAI>(c->GetController());
					con->AttackTarget(target);
				}
			}
		}
	}
}

void ARTSGamePlayerController::GatherResources(IResourceInterface* res) {
	if (selectedUnits.Num() > 0) {
		for (AActor* a : selectedUnits) {
			if (a->IsA(ARTSGameCharacter::StaticClass())) {
				ARTSGameCharacter* c = Cast<ARTSGameCharacter>(a);

				if (c->GetOwningPlayer() != -1) {
					ABaseAI* con = Cast<ABaseAI>(c->GetController());
					con->GatherResource(res);
				}
			}
		}
	}
}

void ARTSGamePlayerController::RightClick()
{
	if (hit.bBlockingHit)
	{
		AActor* targetFound = hit.GetActor();

		bool isDamagable = targetFound->Implements<UDamagableInterface>();
		bool isResource = targetFound->Implements<UResourceInterface>();

		if (isDamagable) {
			IDamagableInterface* damagable = GetDamagable(targetFound);

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
			MoveUnits(hit.Location);
		}
	}
}

void ARTSGamePlayerController::MoveUnits(FVector loc)
{
	if (selectedUnits.Num() > 0) {
		for (AActor* a : selectedUnits) {
			if (a->IsA(ARTSGameCharacter::StaticClass())) {
				ARTSGameCharacter* c = Cast<ARTSGameCharacter>(a);

				if (c->GetOwningPlayer() != -1) {
					ABaseAI* con = Cast<ABaseAI>(c->GetController());
					con->MoveAI(loc);
				}
			}
		}
	}
}

void ARTSGamePlayerController::SelectUnits()
{
	selectionArea->SetWorldLocation(centerMouseLocation);
	selectionArea->SetBoxExtent(selectionSize);

	TArray<AActor*> actors;
	selectionArea->GetOverlappingActors(actors);

	ClearDelegates();

	if (actors.Num() > 0) {
		selectedUnits.Empty();

		for (AActor* a : actors) {
			if (a->IsA(ARTSGameCharacter::StaticClass())) {
				ARTSGameCharacter* character = Cast<ARTSGameCharacter>(a);

				selectedUnits.Add(a);
			}
			else if (a->IsA(ABuilding::StaticClass()))
			{
				selectedUnits.Add(a);
			}
		}

		GenerateUI();
	}
	else {
		inventoryUI->SetVisibility(ESlateVisibility::Hidden);
		characterUI->SetVisibility(ESlateVisibility::Hidden);
		contextUnitUI->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ARTSGamePlayerController::ClearDelegates()
{
	if (selectedUnits.Num() > 0) {
		AActor* a = selectedUnits[0];

		if (a->IsA(ABuilding::StaticClass())) {
			ABuilding* building = Cast<ABuilding>(a);
			building->healthChanged.RemoveAll(characterUI);

			if (a->Implements<UStorageInterface>()) {
				IStorageInterface* si = Cast<IStorageInterface>(a);
				si->inventoryChange.RemoveAll(inventoryUI);
			}
		}
		else if (a->IsA(ARTSGameCharacter::StaticClass()))
		{
			ARTSGameCharacter* c = Cast<ARTSGameCharacter>(a);
			c->inventoryChange.RemoveAll(inventoryUI);
			c->healthChanged.RemoveAll(characterUI);
		}
	}
}

void ARTSGamePlayerController::GenerateUI() {
	if (selectedUnits.Num() > 0) {
		inventoryUI->SetVisibility(ESlateVisibility::Visible);
		characterUI->SetVisibility(ESlateVisibility::Visible);
		contextUnitUI->SetVisibility(ESlateVisibility::Visible);

		AActor* a = selectedUnits[0];

		if (a->IsA(ABuilding::StaticClass())) {
			ABuilding* building = Cast<ABuilding>(a);
			characterUI->SetStats(building->GetBuildingStats());
			building->healthChanged.AddUniqueDynamic(characterUI, &UCharacterDetailsUI::HealthChanged);

			if (building->Implements<UStorageInterface>()) {
				IStorageInterface* si = Cast<IStorageInterface>(building);
				inventoryUI->SetInventory(si->GetInventory());
				inventoryUI->GenerateInventory();
				si->inventoryChange.AddUniqueDynamic(inventoryUI, &UInventoryUI::InventoryChanged);
			}
		}
		else if (a->IsA(ARTSGameCharacter::StaticClass()))
		{
			ARTSGameCharacter* c = Cast<ARTSGameCharacter>(a);
			characterUI->SetStats(c->GetStats());
			inventoryUI->SetInventory(c->GetStats().inventory);
			inventoryUI->GenerateInventory();
			c->healthChanged.AddUniqueDynamic(characterUI, &UCharacterDetailsUI::HealthChanged);
			c->inventoryChange.AddUniqueDynamic(inventoryUI, &UInventoryUI::InventoryChanged);
		}
	}
	else {
		inventoryUI->SetVisibility(ESlateVisibility::Hidden);
		characterUI->SetVisibility(ESlateVisibility::Hidden);
		contextUnitUI->SetVisibility(ESlateVisibility::Hidden);
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