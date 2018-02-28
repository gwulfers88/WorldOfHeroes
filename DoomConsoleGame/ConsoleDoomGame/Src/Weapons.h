/**********************************************************
File Name: Weapons.h
Start Date: 02/20/2018
Mod Date: 02/20/2018
Creator Name: Davin Ross
Contributors Name: Davin Ross
===========================================================
Description:
This file is the header file for the Weapons Class
===========================================================
**********************************************************/
#ifndef WEAPONS_H
#define WEAPONS_H

#include "platform.h"

// Weapons Index
enum WEAPONS {
	FIST = 0,
	PISTOL
};


class Weapons {
public:
	Weapons();
	~Weapons();

	i32 addAmmo(i32);

	// Setters
	void setAmmo(i32);
	
	// Getters
	i32 getAmmo() const;
	i32 getMaxAmmo() const;

private:
	i32 _ammo;
	i32 _maxAmmo;
	i32 _ammoPickup;

	WEAPONS _weaponIndex; // Individual Weapon Index 
};
#endif
