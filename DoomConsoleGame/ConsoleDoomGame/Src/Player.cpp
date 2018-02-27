<<<<<<< HEAD
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

=======
>>>>>>> a466b0535179018ec01b36400d5cbc574e86b7ae
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
		_weapons[i].setAmmo(0);
	}

	setName(name);
}

Player::~Player() {}

// Setter Functions
void Player::setName(i8* name) {

}

void Player::setArmor(i32 armor) {
	if (_armor >= 0 && _armor <= _maxArmor) {
		_armor = armor;
		if (_armor == 0) {

		}
	}
}

void Player::setHealth(i32 health) {
	if (_health >= 0 && _health <= _maxHealth) {
		_health = health;
		if (_health == 0) {

		}
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






