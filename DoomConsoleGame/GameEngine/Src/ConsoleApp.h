

#ifndef CONSOLE_APP_H
#define CONSOLE_APP_H

#include "platform.h"
#include "memory.h"
#include "BaseGame.h"
#include "ConsoleRenderer.h"

class ConsoleApp
{
public:
	ConsoleApp() {}
	~ConsoleApp() {}

	void CreateConsole(size_t memorySize = Megabytes(500), int width = 640, int height = 176, int fontw = 4, int fonth = 4);
	void Run(BaseGame* game);
	void CleanUp();

	int GetScreenWidth() const { return screenWidth; }
	int GetScreenHeight() const { return screenHeight; }
	HANDLE GetHandle() const { return consoleHandle; }
	ConsoleRenderer* GetRenderer() { return &renderer; }	// Wont need this later on here

	MemoryHandle GetPersistantHandle() { return persistantHandle; }
	MemoryHandle GetTransientHandle() { return transientHandle; }

protected:
	HANDLE consoleHandle;
	int screenWidth;
	int screenHeight;
	
	ConsoleRenderer renderer;

	MemoryHandle persistantHandle;
	MemoryHandle transientHandle;
};

#endif

