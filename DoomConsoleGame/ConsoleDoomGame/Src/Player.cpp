#include "Player.h"

Player::Player(i8* name)
	: _health(0),
	_armor(0),
	_maxHealth(100),
	_maxArmor(100), 
	_curWeaponsIndex(FIST), 
	_maxWeapons(2) {

	setName(name);

	// Set All Weapons to 0 Ammo
	for (i32 i = 0; i < _maxWeapons; i++) {
		_ammo[i] = 0;
	}

	// Weapon Inventory
	_weapons[0] = "FIST";
	_weapons[1] = "PISTOL";

	// Max Ammo
	_maxAmmo[0] = 0;
	_maxAmmo[1] = 50;
}


Player::~Player() {}

// Setter Functions
void Player::setName(i8* name) {

}

void Player::setAmmo(i32 ammo) {
	if (_ammo < 0) {

		// Makes Sure to Add Ammo to Weapons That Use Ammo
		if (_curWeaponsIndex != FIST) {
			_ammo[_curWeaponsIndex] = ammo; // Set New Ammo

			// Makes Sure That Ammo Does Not Exceed Max Ammo
			if (_ammo[_curWeaponsIndex] > _maxAmmo[_curWeaponsIndex]) {

				// If Ammo Does Exceed MaxAmmo, Set Ammo to Max Ammo
				_ammo[_curWeaponsIndex] = _maxAmmo[_curWeaponsIndex];
			}
		}
	}
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
	_curWeaponsIndex = newWeaponsIndex;
}

// Getter Functions
i8* Player::getName() const {
	return _name;
}

i32 Player::getAmmo() const {
	return _ammo[_curWeaponsIndex];
}

i32 Player::getArmor() const {
	return _armor;
}

i32 Player::getHealth() const {
	return _health;
}

i32 Player::getMaxAmmo() const {
	return _maxAmmo[_curWeaponsIndex];
}

i8* Player::getCurWeapons() const {
	return _weapons[_curWeaponsIndex];
{


