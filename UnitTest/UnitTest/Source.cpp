#include <iostream>


int main() {
	int ammo = 5;
	int _ammoPickup = 3;
	int _maxAmmo = 10;

	if (ammo + _ammoPickup < _maxAmmo) {

		// Adds the ammo that is picked up to the current ammo
		ammo += _ammoPickup;
		if (ammo >= _maxAmmo) {

			// If the current ammo exceeds the maximum allowed ammo, set the current ammo to the max ammo.
			ammo = _maxAmmo;
		}
		std::cout << ammo << std::endl;
	}
	system("pause");
	return 0;
}

