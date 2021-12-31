#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResourceInterface.h"
#include "Resource.generated.h"


UCLASS()
class RTSGAME_API AResource : public AActor, public IResourceInterface
{
	GENERATED_BODY()
	
public:	
	AResource();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	FResourceStats stats;

public:	
	virtual void TakeResources(int32 amount) override;
	virtual EResourceType GetType() override;
	virtual int32 GetAmount() override;
};