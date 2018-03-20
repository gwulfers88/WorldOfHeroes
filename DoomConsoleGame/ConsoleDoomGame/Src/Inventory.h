/**********************************************************
File Name: Inventory.h
Project Name: Doom Game
Start Date: 02/25/2018
Mod Date: 02/20/2018
Creator Name: Davin Ross
Contributors Name: Davin Ross
===========================================================
Description:
This file is the header file for the Inventory Class
===========================================================
**********************************************************/
#include "platform.h"

#ifndef INVENTORY_H
#define INVENTORY_H

// Keys Index
enum KEYS {
	RED = 0,
	BLUE,
	GREEN
};

class Inventory {
public:
	Inventory();
	~Inventory();

	//i32 storedDoorKeys[3];

	// Setters
	void setDoorKeyIndex(KEYS);

	// Getters
	KEYS getDoorKeyIndex() const;

private:
	KEYS _doorKeyIndex;
};
#endif

