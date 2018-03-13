/**********************************************************
File Name: Player.h
Project Name: Doom Game
Start Date: 01/23/2018
Mod Date: 03/12/2018
Creator Name: Davin Ross
Contributors Name: Davin Ross
===========================================================
Description: 
	This file is the header file for the Player Class
===========================================================
**********************************************************/
#ifndef PLAYER_H
#define PLAYER_H

#include "platform.h"
#include "Weapons.h"

class Player {
public:
	// Unit Test Only
	Player();

	Player(i8* name);
	~Player(void);

	void addWeaponToInventory(Weapons);
	//void deleteWeaponFromInventory(Weapons);

	i32 playerDamage(i32);
	i32 addHealth(i32);
	
	i32 addArmor(i32);

	// Setters
	void setHealth(i32);
	void setMaxHealth(i32 a = 100);

	void setArmor(i32);
	void setMaxArmor(i32 a = 100);

	void setCurWeapons(WEAPONS newWeaponsIndex);
	
	// Getters
	i32 getHealth() const;
	i32 getArmor() const;
	i8* getName() const;
	Weapons getCurWeapons() const;
	
private:
	// Current Health and Armor 	
	i32 _health;
	i32 _armor;

	// Max Health and Armor
	i32 _maxHealth;
	i32 _maxArmor;

	void setName(i8*);
	i8* _name;

	// Weapons Slots
	WEAPONS _curWeaponsIndex; // Curent Index
	i32 _maxWeapons; // Max Number of Weapons
	Weapons _weapons[2]; // Weapons Array
};
#endif