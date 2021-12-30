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
	UFUNCTION(BlueprintCallable, Category = "Character Stats")
		float GetHealth();

	FCharacterStats GetStats() const { return stats; }
	void SetStats(FCharacterStats val) { stats = val; }
private:
	FCharacterStats stats;

};