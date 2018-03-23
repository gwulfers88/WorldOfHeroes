/**********************************************************
File Name: Weapons.h
Project Name: Doom Game
Start Date: 02/20/2018
Mod Date: 03/12/2018
Creator Name: Davin Ross
Contributors Name: Davin Ross and George Wulfers
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
	NONE = -1,
	FIST,
	PISTOL
};

class Weapons {
public:
	Weapons();
	~Weapons();

	i32 addAmmo(i32);
	i32 subtractAmmo(i32);

	i8* weaponName;

	// Setters
	void setAmmo(i32);
	void setWeaponIndex(WEAPONS);
	void setMaxAmmo(i32);
	void setWeaponName(i8*);
	
	// Getters
	i32 getAmmo() const;
	i32 getMaxAmmo() const;
	i8* getWeaponName() const;
	WEAPONS getWeaponIndex() const;

private:
	i32 _ammo;
	i32 _maxAmmo;
	
	WEAPONS _weaponIndex = NONE; // Individual Weapon Index 
};
#endif
