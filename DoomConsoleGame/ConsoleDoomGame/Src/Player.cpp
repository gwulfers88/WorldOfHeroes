#include "Player.h"

Player::Player(i8* name)
	: _health(0),
	_armor(0),
	_maxHealth(100),
	_maxArmor(100)
{}


Player::~Player() {}

// Setter Functions
void Player::setAmmo(i32 ammo) {

}

void Player::setArmor(i32 armor) {

}

void Player::setHealth(i32 health) {

}

// Getter Functions
i32 Player::getAmmo() {
	return 0;
}

i32 Player::getArmor() {
	return 0;
}

i32 Player::getHealth() {
	return 0;
}

