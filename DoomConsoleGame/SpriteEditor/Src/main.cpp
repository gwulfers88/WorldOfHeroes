/*
	main.cpp
	George Wulfers
	01/29/2018

	This is the entry point of the sprite editor 
*/
#pragma comment(lib, "GameEngine_Debug.lib")
#pragma comment(lib, "Winmm.lib")

#include "ConsoleApp.h"
#include "SpriteEditor.h"

void GetInput(char *buffer, u32 size)
{
	int i = 0;
	int ch = 0;
	for (i = 0; 
		i < (size - 1) && 
		(ch = getchar()) != EOF && 
		ch != '\n'; 
		i++)
	{
		buffer[i] = (char)ch;
	}

	buffer[i] = 0;
}

int main()
{
	char filename[256];
	int W = 100;
	int H = 100;

	char load[4];
	bool loadFile = false;
	printf("Would you like to load an image?Yes, No: ");
	GetInput(load, ArrayCount(load));
	if (strcmp(load, "Yes") == 0)
	{
		printf("loading image\n");

		getchar();

		printf("Enter a filename:\n");
		GetInput(filename, ArrayCount(filename));
		printf("input: %s\n", filename);
		loadFile = true;
	}
	else if (strcmp(load, "No") == 0)
	{
		printf("not loading image\n");
		getchar();

		char imageWidth[256];
		char imageHeight[256];
		printf("Please choose an image size. Width: ");
		GetInput(imageWidth, ArrayCount(imageWidth));
		getchar();

		printf("Please choose an image size. Height: ");
		GetInput(imageHeight, ArrayCount(imageHeight));

		if (imageWidth[0] >= '0' && 
			imageWidth[0] <= '9' &&
			imageHeight[0] >= '0' &&
			imageHeight[0] <= '9')
		{
			W = atoi(imageWidth);
			H = atoi(imageHeight);
		}

		getchar();

		printf("Enter a filename:\n");
		GetInput(filename, ArrayCount(filename));

		getchar();
	}

	ConsoleApp App = {};
	App.CreateConsole(Megabytes(100), 200, 100, 10, 12);
	SpriteEditor editor = {};
	editor.SetFilename(filename, loadFile);
	editor.SetImageSize(W, H);
	MemoryHandle persistant = App.GetPersistantHandle();
	Assert(persistant);
	MemoryHandle transient = App.GetTransientHandle();
	Assert(transient);
	editor.SetMemoryHandles(persistant, transient);
	App.Run(&editor);
	App.CleanUp();

	return 0;
}