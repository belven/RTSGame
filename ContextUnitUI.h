#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterDetailsUI.h"
#include "InventoryUI.h"
#include "ContextUnitUI.generated.h"

UCLASS()
class RTSGAME_API UContextUnitUI : public UUserWidget
{
	GENERATED_BODY()
private:
	UCharacterDetailsUI* cd;
	UInventoryUI* inventoryUI;
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Context UI")
		void GenerateUI(UCharacterDetailsUI* inCharacterDetailsUI, UInventoryUI* inInventoryUI);

	UFUNCTION(BlueprintCallable, Category = "Context UI")
		UCharacterDetailsUI* GetCharacterDetails() const { return cd; }

	UFUNCTION(BlueprintCallable, Category = "Context UI")
		void SetCharacterDetails(UCharacterDetailsUI* val) { cd = val; }

	UFUNCTION(BlueprintCallable, Category = "Context UI")
		UInventoryUI* GetInventoryUI() const { return inventoryUI; }

	UFUNCTION(BlueprintCallable, Category = "Context UI")
		void SetInventoryUI(UInventoryUI* val) { inventoryUI = val; }
};