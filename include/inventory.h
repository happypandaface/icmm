#ifndef INVENTORY_H_
#define INVENTORY_H_

#include "item.h"

struct InvSlot;

struct InvSlot
{
	Item* item;
	struct InvSlot* last;
	struct InvSlot* next;
};

#endif