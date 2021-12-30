#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ResourceInterface.h"
#include "Resource.h"
#include "RTSOverseerer.generated.h"

UCLASS()
class RTSGAME_API ARTSOverseerer : public ACharacter
{
	GENERATED_BODY()

public:
	ARTSOverseerer();

	int32 GetTeam() const { return team; }
	void SetTeam(int32 val) { team = val; }
	int32 GetOwningPlayer() const { return owningPlayer; }
	void SetOwningPlayer(int32 val) { owningPlayer = val; }
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	void AddResource(EResourceType type, int32 amount);
	void RemoveResource(EResourceType type, int32 amount);
	int32 GetResource(EResourceType type);

	int32 team;
	int32 owningPlayer;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		TMap <EResourceType, FResourceStats> resourses;

public:
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	virtual void Tick(float DeltaTime) override;
};