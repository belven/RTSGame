#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StorageInterface.generated.h"

USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		FString name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int32 amount;
};

USTRUCT(BlueprintType)
struct FInventory
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		TArray<FItem> items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int32 capacity;
};

UINTERFACE(MinimalAPI)
class UStorageInterface : public UInterface
{
	GENERATED_BODY()
};

class RTSGAME_API IStorageInterface
{
	GENERATED_BODY()

public:
	virtual TArray<FItem>& GetItems();
	virtual void AddItem(FString name, int32 amount);
	virtual void RemoveItem(FString name, int32 amount);
	//virtual FItem GetItemByName(FString name);
protected:
		FInventory inventory;
};
