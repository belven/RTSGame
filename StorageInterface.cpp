#include "StorageInterface.h"

TArray<FItem>& IStorageInterface::GetItems()
{
	return inventory.items;
}

void IStorageInterface::AddItem(FString name, int32 amount)
{
	bool itemFound = false;

	for (FItem item : GetItems()) {
		if (item.name.Equals(name)) {
			item.amount += amount;
			break;
		}
	}

	if (!itemFound) {
		FItem item;
		item.name = name;
		item.amount = amount;
		GetItems().Add(item);
	}
}

void IStorageInterface::RemoveItem(FString name, int32 amount)
{
	for (FItem item : GetItems()) {
		if (item.name.Equals(name)) {
			item.amount -= amount;
			break;
		}
	}
}
