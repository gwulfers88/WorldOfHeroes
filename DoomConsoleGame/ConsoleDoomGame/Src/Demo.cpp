/*******************************************************
FileName: Entry.cpp
StartDate: 01/17/2018
ModDate: 01/18/2018
CreatorName: George Wulfers
ContributorsName: George Wulfers
===================================================
This is the entry point to our progrqam.
*******************************************************/

#pragma comment(lib, "GameEngine_Debug.lib")
#pragma comment(lib, "Winmm.lib")

#include "ConsoleApp.h"
#include "DemoGame.h"
#include "Player.h"

// Inventory Vars
int ammo = 0;
int shield = 0;
int health = 0;


//int argCount -- Used to determine the number of arguments being passed to the application
//char** args  -- Array list of arguments entered.
int main(int argCount, char** args)
{
	for (int i = 0; i < argCount; ++i)
	{
		printf("argCount: %d -- args: %s \n", i, args[i]);
	}

	ConsoleApp App;
	App.CreateConsole(Megabytes(50), 640, 176, 4, 6);
	DemoGame game = {};
	App.Run(&game);
	App.CleanUp();

	return 0;
}