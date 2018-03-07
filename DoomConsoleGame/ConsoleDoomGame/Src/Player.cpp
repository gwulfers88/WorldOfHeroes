/**********************************************************
File Name: Player.cpp
Start Date: 01/23/2018
Mod Date: 02/20/2018
Creator Name: Davin Ross
Contributors Name: Davin Ross
===========================================================
Description:
	This file is the source file for the Player Class
===========================================================
**********************************************************/
#include "Player.h"

Player::Player(i8* name)
	: _health(0),
	_armor(0),
	_maxHealth(100),
	_maxArmor(100),
	_curWeaponsIndex(FIST),
	_maxWeapons(2) {

	// Set All Weapons to 0 Ammo
	for (i32 i = 0; i < _maxWeapons; i++) {
		if (i == 0) {
			_weapons[i].setWeaponIndex(WEAPONS::FIST);
		}
		else if (i == 1) {
			_weapons[i].setWeaponIndex(WEAPONS::PISTOL);
		}
		_weapons[i].setAmmo(0);
	}

	setName(name);
}

Player::~Player() {}

// For Unit Test Only
Player::Player() {}

void Player::addWeaponToInventory(Weapons w) {
	if (_weapons[w.getWeaponIndex()].getWeaponIndex() < 0) {
		_weapons[w.getWeaponIndex()] = w;
	}
}

// Setter Functions
void Player::setName(i8* name) {

}

void Player::setArmor(i32 armor) {
	// Check to make sure armor does not exceed 100 or drop below 0
	if (armor >= 0 && armor <= _maxArmor) {
		_armor = armor; // Set new armor
	}
	// If armor does exceed _maxArmor(100), set armor to maxArmor
	else if (armor > _maxArmor) {
		_armor = _maxArmor;
	}
}

void Player::setMaxArmor(i32 maxArmor) {
	// Check to make sure that maxArmor exceeds 0
	if (maxArmor > 0) {
		_maxArmor = maxArmor; // Set new maxArmor
	}
	// If maxArmor is less than 0, set maxArmor to 100
	else {
		_maxArmor = 100;
	}
}

void Player::setHealth(i32 health) {
	// Check to make sure health does not exceed 100 or drop below 0
	if (health >= 0 && health <= _maxHealth) {
		_health = health; // Set new health
	}
	// If health does exceed _maxHealth(100), set health to _maxHealth
	else if (health > _maxHealth) {
		_health = _maxHealth;
	}
}

void Player::setMaxHealth(i32 maxHealth) {
	// Check to make sure taht maxHealth exceeds 0
	if (maxHealth > 0) {
		_maxHealth = maxHealth; // Setr new maxHealth
	}
	// If maxHealth is less than 0, set maxHealth to 100
	else {
		_maxHealth = 100;
	}
}


void Player::setCurWeapons(WEAPONS newWeaponsIndex) {
	_curWeaponsIndex = newWeaponsIndex; // _curWeaponIndex does not exist in player.h
}

// Getter Functions
i8* Player::getName() const {
	return _name;
}

i32 Player::getArmor() const {
	return _armor;
}

i32 Player::getHealth() const {
	return _health;
}

Weapons Player::getCurWeapons() const {
	return _weapons[_curWeaponsIndex];
}






