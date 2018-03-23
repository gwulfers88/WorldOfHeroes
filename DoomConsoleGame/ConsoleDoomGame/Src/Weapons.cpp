/**********************************************************
File Name: Weapons.cpp
Project Name: Doom Game
Start Date: 02/20/2018
Mod Date: 03/12/2018
Creator Name: Davin Ross
Contributors Name: Davin Ross and George Wulfers
===========================================================
Description:
This file is the source file for the Weapons Class
===========================================================
**********************************************************/
#include "Weapons.h"
#include "Memory.h"

Weapons::Weapons() {}

Weapons::~Weapons() {}

// When the player picks up ammo from the level
i32 Weapons::addAmmo(i32 ammoPickup) {

	// Check to see if amm is less than max ammo.
	if (_ammo + ammoPickup < _maxAmmo) {

		// Adds the ammo that is picked up to the current ammo
		_ammo += ammoPickup;
	}
	else {
		_ammo = _maxAmmo; // Set ammo to max ammo.
	}
	return _ammo;
}

i32 Weapons::subtractAmmo(i32 expendedAmmo) {

	// Check to see if ammo - expended ammo is less than ammo.
	if (_ammo - expendedAmmo < _ammo) {
		_ammo -= expendedAmmo; // Subtract expended ammo from ammo.
	}
	return _ammo;
}

// Setter Functions
void Weapons::setAmmo(i32 ammo) {
	if (ammo < _maxAmmo) {

		// Makes Sure to Add Ammo to Weapons That Use Ammo
		if (_weaponIndex != FIST) {
			_ammo = ammo; // Set New Ammo
		}
	} 
	else {
		// If Ammo Does Exceed MaxAmmo, Set Ammo to Max Ammo
		_ammo = _maxAmmo;
	}
}

void Weapons::setMaxAmmo(i32 maxAmmo) {
	// Check to make sure that you are setting max ammo to a weapon that uses ammo 
	if (_weaponIndex != FIST) {
		_maxAmmo = maxAmmo;
	}
}

void Weapons::setWeaponIndex(WEAPONS w) {
	_weaponIndex = w;
}

void Weapons::setWeaponName(i8* wn) {
	// Check to see if there is a weapon index
	if (_weaponIndex != NONE) {
		
		// Creates an array that holds weapon name
		i32 strSize = strlen(wn) + 1;
		weaponName = CreateArray(Memory::GetPersistantHandle(), i8, strSize);
		strcpy(weaponName, wn);
		weaponName[strSize - 1] = '\0';
	}
}

// Getter Functions
WEAPONS Weapons::getWeaponIndex() const {
	return _weaponIndex;
}

i32 Weapons::getAmmo() const {
	return _ammo;
}

i32 Weapons::getMaxAmmo() const {
	return _maxAmmo;
}
i8* Weapons::getWeaponName() const {
	return weaponName;
}


