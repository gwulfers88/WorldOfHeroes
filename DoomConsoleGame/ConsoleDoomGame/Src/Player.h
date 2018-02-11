#pragma once
#include "platform.h"

class Player {
public:

	Player(i8* name);
	~Player(void);

	// Setters
	void setAmmo(i32);
	void setHealth(i32);
	void setArmor(i32);

	// Getters
	i32 getAmmo();
	i32 getHealth();
	i32 getArmor();

private:

	// Current Ammo, Health and Armor 
	i32 _ammo;
	i32 _health;
	i32 _armor;

	// Max Ammo, Health and Armor
	i32 _maxAmmo;
	i32 _maxHealth;
	i32 _maxArmor;

};
