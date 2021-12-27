#include "RTSOverseerer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"

ARTSOverseerer::ARTSOverseerer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->MaxWalkSpeed = 2000;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 3000.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	FResourceStats wood;
	wood.resourceType = EResourceType::Wood;
	wood.amount = 50;

	resourses.Add(EResourceType::Wood, wood);

	FResourceStats gold;
	gold.resourceType = EResourceType::Gold;
	gold.amount = 50;

	resourses.Add(EResourceType::Gold, gold);

	FResourceStats stone;
	stone.resourceType = EResourceType::Stone;
	stone.amount = 50;

	resourses.Add(EResourceType::Stone, stone);

	FResourceStats food;
	food.resourceType = EResourceType::Food;
	food.amount = 50;

	resourses.Add(EResourceType::Food, food);
}

void ARTSOverseerer::AddResource(EResourceType type, int32 amount)
{
	FResourceStats resource = *resourses.Find(type);
	resource.amount += amount;
}

void ARTSOverseerer::RemoveResource(EResourceType type, int32 amount)
{
	FResourceStats resource = *resourses.Find(type);
	resource.amount -= amount;
}

int32 ARTSOverseerer::GetResource(EResourceType type)
{
	FResourceStats resource = *resourses.Find(type);
	return resource.amount;
}

void ARTSOverseerer::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARTSOverseerer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}