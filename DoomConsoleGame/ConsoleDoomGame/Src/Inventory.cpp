/**********************************************************
File Name: Inventory.cpp
Project Name: Doom Game
Start Date: 02/25/2018
Mod Date: 02/27/2018
Creator Name: Davin Ross
Contributors Name: Davin Ross
===========================================================
Description:
This file is the source file for the Inventory Class
===========================================================
**********************************************************/
#include "Inventory.h"

Inventory::Inventory() {}

Inventory::~Inventory() {}

// Setters
void Inventory::setDoorKeyIndex(KEYS k) {
	_doorKeyIndex = k;
}

// Getters
KEYS Inventory::getDoorKeyIndex() const {
	return _doorKeyIndex;
}


