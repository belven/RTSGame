#include "CharacterDetailsUI.h"
#include "Resource.h"

float UCharacterDetailsUI::GetHealth()
{
	return stats.currentHealth;
}

FString UCharacterDetailsUI::GetName()
{
	return stats.unitName;
}