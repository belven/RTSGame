#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnitButtons.generated.h"


UCLASS()
class RTSGAME_API UUnitButtons : public UUserWidget
{
	GENERATED_BODY()
public:
	//typedef void (UUnitButtons::* ButtonFunction)(void);
	//void AddButton(ButtonFunction newFunctions);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Unit Controls")
		void GenerateButtons();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Unit Controls")
		void UpdateButtonsEnabled();

	UFUNCTION(BlueprintCallable, Category = "Unit Controls")
		AActor* GetOwningUnit() const { return onwingUnit; }

	UFUNCTION(BlueprintCallable, Category = "Unit Controls")
		void SetOwningUnit(AActor* val) { onwingUnit = val; }
private:
	AActor* onwingUnit;
	//TArray<ButtonFunction> FunctionsToRun;
};
