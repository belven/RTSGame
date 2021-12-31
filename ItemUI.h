#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enums.h"
#include "ItemUI.generated.h"

UCLASS()
class RTSGAME_API UItemUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Item")
		void GenerateItem(FItem newItem);

	UFUNCTION(BlueprintCallable, Category = "Item")
		FItem GetItem() const { return item; }

	UFUNCTION(BlueprintCallable, Category = "Item")
		void SetItem(FItem val) { item = val; }
private:
	FItem item;

};
