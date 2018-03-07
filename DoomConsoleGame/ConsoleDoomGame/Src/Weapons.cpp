/**********************************************************
File Name: Weapons.cpp
Start Date: 02/20/2018
Mod Date: 02/20/2018
Creator Name: Davin Ross
Contributors Name: Davin Ross
===========================================================
Description:
This file is the source file for the Weapons Class
===========================================================
**********************************************************/
#include "Weapons.h"

Weapons::Weapons() {}

Weapons::~Weapons() {}

// When the player picks up ammo from the level
i32 Weapons::addAmmo(i32 ammoPickup) {
	if (_ammo + ammoPickup < _maxAmmo) {

		// Adds the ammo that is picked up to the current ammo
		_ammo += ammoPickup;
	}
	else {
		_ammo = _maxAmmo;
	}
	return _ammo;
}

// Setter Functions
void Weapons::setAmmo(i32 ammo) {
	if (ammo <= 0) {

		// Makes Sure to Add Ammo to Weapons That Use Ammo
		if (_weaponIndex != FIST) {
			_ammo = ammo; // Set New Ammo

			// Makes Sure That Ammo Does Not Exceed Max Ammo
			if (_ammo > _maxAmmo) {

				// If Ammo Does Exceed MaxAmmo, Set Ammo to Max Ammo
				_ammo = _maxAmmo;
			}
		}
	}
}

void Weapons::setMaxAmmo(i32 maxAmmo) {
	if (_weaponIndex != FIST) {
		_maxAmmo = maxAmmo;
	}
}

void Weapons::setWeaponIndex(WEAPONS w) {
	_weaponIndex = w;
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


