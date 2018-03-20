/**********************************************************
File Name: Player.cpp
Project Name: Doom Game
Start Date: 01/23/2018
Mod Date: 03/12/2018
Creator Name: Davin Ross
Contributors Name: Davin Ross
===========================================================
Description:
	This file is the source file for the Player Class
===========================================================
**********************************************************/
#include "Player.h"
#include "memory.h"

WEAPONS Player::_curWeaponsIndex = WEAPONS::FIST;

Player::Player(i8* name)
	: _health(0),
	_armor(0),
	_maxHealth(100),
	_maxArmor(100),
	_maxWeapons(2) {

	setName(name);
}

Player::~Player() {
	
}

// For Unit Test Only
Player::Player()
: GameObject()
{
	InitPlayer();
}

void Player::InitPlayer()
{
	setMaxHealth();
	setMaxArmor();
	setHealth(100);
	setArmor(50);

	_maxWeapons = 2;

	dir = {};
}

void Player::Update(r32 deltaTime)
{
	GameObject::Update(deltaTime);
	Move(dir, deltaTime);
}

void Player::addWeaponToInventory(Weapons* w) {
	_weapons[w->getWeaponIndex()] = w;
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

i32 Player::addArmor(i32 addArmor) {

	// Check to see if armor is less than max armor. 
	if (_armor + addArmor < _maxArmor) {
		_armor += addArmor; // Add armor and addArmor
	}
	else {
		_armor = _maxArmor; // Set armor to max armor
	}
	return _armor;
}

// Player Health Functions
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

i32 Player::playerDamage(i32 damage) {
	i32 leftoverDamage = _armor - damage;
	if (leftoverDamage <= 0) {
		damage -= leftoverDamage*-1;
		_health -= leftoverDamage*-1;
	}
	// Check to see if armor <= 0, and if damage is being done. 
	if (_armor - damage >= 0 && _armor > 0) {
		_armor -= damage; // Subtract damage from health.

		if (_armor < 0) {
			leftoverDamage = _armor * -1;
			_health -= leftoverDamage;
		}
	}
	// Check to see if armor > 0
	else {
		_health -= damage; // subtract damage from armor.
	}
	return _health, _armor;
}

i32 Player::addHealth(i32 addHealth) {
	
	// Check to see if health is less than max health.
	if (_health + addHealth < _maxHealth) {
		_health += addHealth; // add health and addHealth
	}
	else {
		_health = _maxHealth; // Set health to max health.
	}
	return _health;
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

WEAPONS Player::getCurWeapons() {
	return _curWeaponsIndex;
}






