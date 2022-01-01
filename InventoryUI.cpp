#include "InventoryUI.h"

void UInventoryUI::InventoryChanged(FInventory newInventory)
{
	inventory = newInventory;
	GenerateInventory();
}
