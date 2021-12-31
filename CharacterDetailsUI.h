#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RTSGameCharacter.h"
#include "CharacterDetailsUI.generated.h"

UCLASS()
class RTSGAME_API UCharacterDetailsUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Unit Stats")
		FString GetName();

	UFUNCTION(BlueprintCallable, Category = "Unit Stats")
		float GetHealth();

	FUnitStats GetStats() const { return stats; }
	void SetStats(FUnitStats val) { stats = val; }
private:
	FUnitStats stats;

};