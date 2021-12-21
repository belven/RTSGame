#pragma once

#include "CoreMinimal.h"
#include "ResourceInterface.h"
#include "Resource.h"
#include "RTSGameCharacter.h"
#include "Animal.generated.h"

UCLASS()
class RTSGAME_API AAnimal : public ARTSGameCharacter, public IResourceInterface
{
	GENERATED_BODY()

public:
	AAnimal();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		FResourceStats resourceStats;

	virtual void TakeResources(int32 amount) override;
	virtual EResourceType GetType() override;
	virtual int32 GetAmount() override;
};