#include "DamagableInterface.h"

void IDamagableInterface::TakeDamage(float damage)
{

}

float IDamagableInterface::GetHealth()
{
	return 1;
}

float IDamagableInterface::GetMaxHealth()
{
	return 1;

}

int32 IDamagableInterface::GetTeam()
{
	return 1;
}

int32 IDamagableInterface::GetOwner()
{
	return 1;
}
