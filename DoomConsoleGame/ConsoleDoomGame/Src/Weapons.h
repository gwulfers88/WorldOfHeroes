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
#include "Player.h"

class Weapons {
public:
	Weapons();
	~Weapons();

	// i32 addAmmo(i32); // Commented this because the definition was commented.

	// Setters
	void setAmmo(i32);
	
	// Getters
	i32 getAmmo() const;
	i32 getMaxAmmo() const;
		
private:
	i32 _ammo;
	i32 _maxAmmo;

	WEAPONS _weaponIndex; // Individual Weapon Index 
};
#endif
