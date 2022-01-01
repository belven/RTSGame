#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enums.h"
#include "InventoryUI.generated.h"

UCLASS()
class RTSGAME_API UInventoryUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Inventory")
		void GenerateInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		FInventory GetInventory() const { return inventory; }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void SetInventory(FInventory val) { inventory = val; }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void InventoryChanged(FInventory newInventory);

private:
	FInventory inventory;
};
