/**********************************************************
File Name: Player.h
Start Date: 01/23/2018
Mod Date: 02/20/2018
Creator Name: Davin Ross
Contributors Name: Davin Ross
===========================================================
Description:
This file is the header file for the Player Class
===========================================================
**********************************************************/
#include "stdafx.h"
#include "CppUnitTest.h"
#include <assert.h>
#include "../ConsoleDoomGame/Src/Player.h"
#include "../ConsoleDoomGame/Src/Weapons.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_Davin {		
	TEST_CLASS(UnitTest1) {
	public:
		
		TEST_METHOD(TestMethod1) {
			Weapons w;
			w.setWeaponIndex(WEAPONS::PISTOL);
			w.setMaxAmmo(15);
			w.setAmmo(0);
			w.addAmmo(9);

			Assert::AreEqual(9, w.getAmmo());
		}
	};

	TEST_CLASS(UnitTest2) {
	public:

		TEST_METHOD(TestMethod2) {
			Weapons w;
			w.setWeaponIndex(WEAPONS::PISTOL);
			w.setMaxAmmo(15);
			w.setAmmo(0);
			w.addAmmo(16);

			Assert::AreEqual(15, w.getAmmo());
		}
	};

	TEST_CLASS(UnitTest3) {
	public:

		TEST_METHOD(TestMethod3) {
			Player p;
			Weapons w;
			Weapons w2;

			w.setWeaponIndex(WEAPONS::FIST);
			w.setMaxAmmo(0);
			w.setAmmo(0);
			w.addAmmo(0);

			w2.setWeaponIndex(WEAPONS::PISTOL);
			w2.setMaxAmmo(15);
			w2.setAmmo(5);
			w2.addAmmo(7);
			
			p.addWeaponToInventory(w);
			p.addWeaponToInventory(w2);

			p.setCurWeapons(WEAPONS::FIST);
			Weapons equiped1 = p.getCurWeapons();

			p.setCurWeapons(WEAPONS::PISTOL);
			Weapons equiped2 = p.getCurWeapons();

			Assert::AreEqual((i32)FIST, (i32)equiped1.getWeaponIndex());
			Assert::AreEqual((i32)PISTOL, (i32)equiped2.getWeaponIndex());
		}
	};

	TEST_CLASS(UnitTest4) {
	public:

		TEST_METHOD(TestMethod4) {
			Player p;

			p.setMaxArmor();
			p.setMaxHealth();
			p.setHealth(100);
			p.setArmor(100);

			Assert::AreEqual(100, p.getHealth());
			Assert::AreEqual(100, p.getArmor());
		}
	};
}