#include "CharacterDetailsUI.h"
#include "Resource.h"

float UCharacterDetailsUI::GetHealth()
{
	return stats.currentHealth;
}

void UCharacterDetailsUI::HealthChanged(float newHealth)
{
	stats.currentHealth = newHealth;
	UpdateHealth();
}

FString UCharacterDetailsUI::GetName()
{
	return stats.unitName;
}