/**********************************************************
File Name: unittest1.cpp
Project Name: Doom Game
Start Date: 03/07/2018
Mod Date: 03/12/2018
Creator Name: Davin Ross
Contributors Name: Davin Ross
===========================================================
Description:
This file is used for Unit Testing
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
			w.subtractAmmo(4);

			Assert::AreEqual(5, w.getAmmo());
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
			w.subtractAmmo(5);

			Assert::AreEqual(10, w.getAmmo());
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
			p.setArmor(50);
			p.addHealth(0);
			p.addArmor(0);
			p.playerDamage(75);

			Assert::AreEqual(75, p.getHealth());
			Assert::AreEqual(0, p.getArmor());
		}
	};
}