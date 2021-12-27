#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResourceInterface.h"
#include "Resource.generated.h"

USTRUCT(BlueprintType)
struct FResourceStats
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float  amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		EResourceType resourceType;
};

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