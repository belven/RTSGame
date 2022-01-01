#include "StorageInterface.h"

TArray<FItem>& IStorageInterface::GetItems()
{
	return inventory.items;
}

void IStorageInterface::AddItem(FString name, int32 amount)
{
	bool itemFound = false;

	for (FItem& item : GetItems()) {
		if (item.itemName.Equals(name)) {
			item.amount += amount;
			itemFound = true;
			break;
		}
	}

	if (!itemFound) {
		FItem item;
		item.itemName = name;
		item.amount = amount;
		GetItems().Add(item);
	}

	inventoryChange.Broadcast(inventory);
}

void IStorageInterface::RemoveItem(FString name, int32 amount)
{
	for (FItem item : GetItems()) {
		if (item.itemName.Equals(name)) {
			item.amount -= amount;
			break;
		}
	}
	inventoryChange.Broadcast(inventory);
}
