#pragma once
#include "platform.h"

// Weapons Index
enum WEAPONS {
	FIST = 0,
	PISTOL
};

class Player {
public:

	Player(i8* name);
	~Player(void);

	// Setters
	void setAmmo(i32);
	void setHealth(i32);
	void setArmor(i32);
	void setCurWeapons(WEAPONS newWeaponsIndex);

	// Getters
	i32 getAmmo() const;
	i32 getHealth() const;
	i32 getArmor() const;
	i32 getMaxAmmo() const;
	i8* getName() const;
	i8* getCurWeapons() const;

private:

	// Current Ammo, Health and Armor 
	i32 _ammo[2];
	i32 _health;
	i32 _armor;

	// Max Ammo, Health and Armor
	i32 _maxAmmo[2];
	i32 _maxHealth;
	i32 _maxArmor;

	void setName(i8*);
	i8* _name;

	// Weapons Slots
	WEAPONS _curWeaponsIndex; // Curent Index
	i32 _maxWeapons; // Max Number of Weapons
	i8* _weapons[2]; // Weapons Array
};
