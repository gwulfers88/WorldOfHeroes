/*
	main.cpp
	George Wulfers
	01/29/2018

	This is the entry point of the sprite editor 
*/
#pragma comment(lib, "GameEngine_Debug.lib")

#include "ConsoleApp.h"
#include "SpriteEditor.h"

int main()
{
	ConsoleApp App = {};
	App.CreateConsole(Megabytes(100), 400, 200, 10, 10);
	SpriteEditor editor = {};
	MemoryHandle persistant = App.GetPersistantHandle();
	Assert(persistant);
	MemoryHandle transient = App.GetTransientHandle();
	Assert(transient);
	editor.SetMemoryHandles(persistant, transient);
	App.Run(&editor);
	App.CleanUp();

	return 0;
}