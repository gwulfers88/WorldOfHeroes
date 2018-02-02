#include "ConsoleApp.h"

void ConsoleApp::CreateConsole(size_t memorySize, int width, int height, int fontw, int fonth)
{
	screenWidth = width;
	screenHeight = height;
	
	// Initialize our memory pools.
	Memory::InitializeMemory(memorySize);

	persistantHandle = Memory::InitializeChunk((int)(memorySize * 0.5f));
	transientHandle = Memory::InitializeChunk((int)(memorySize * 0.5f));

	// Get handle to a new console window that we can read and write from.
	consoleHandle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, 0, CONSOLE_TEXTMODE_BUFFER, 0);

	retry:
	// Set initial window size to the smalled possible. this is needed to be able to set a propper size for the window.
	SMALL_RECT rect = { 0, 0, 1, 1 };
	Assert(SetConsoleWindowInfo(consoleHandle, TRUE, &rect));

	// Set the desired buffer size for the window.
	Assert(SetConsoleScreenBufferSize(consoleHandle, { (short)screenWidth, (short)screenHeight }));

	// Let windows know that we want to use our new window.
	Assert(SetConsoleActiveScreenBuffer(consoleHandle));

	// Set the font size now that the screen buffer has been assigned to the console
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = fontw;
	cfi.dwFontSize.Y = fonth;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");

	Assert(SetCurrentConsoleFontEx(consoleHandle, false, &cfi));

	// Check to see if the current buffer is the correct size for our window.
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	GetConsoleScreenBufferInfo(consoleHandle, &consoleInfo);
	if (screenWidth > consoleInfo.dwMaximumWindowSize.X)
	{
		//MessageBox(0, L"Width is greater than max Width\n", L"ERROR!", MB_OK);
		screenWidth = consoleInfo.dwMaximumWindowSize.X;
		goto retry;
	}
	if (screenHeight > consoleInfo.dwMaximumWindowSize.Y)
	{
		//MessageBox(0, L"Height is greater than max Height\n", L"ERROR!", MB_OK);
		screenHeight = consoleInfo.dwMaximumWindowSize.Y;
		goto retry;
	}

	// set the actual window size.
	rect = { 0, 0, (short)screenWidth - 1, (short)screenHeight - 1 };
	Assert(SetConsoleWindowInfo(consoleHandle, TRUE, &rect));

	// Let windows know that we want to handle some events like input and mouse input.
	SetConsoleMode(consoleHandle, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

	// set title for window.
	SetConsoleTitleW(L"Console Game Engine");

	ScreenBuffer *screen = CreateObject(persistantHandle, ScreenBuffer);
	screen->Width = screenWidth;
	screen->Height = screenHeight;
	// Create our buffers
	screen->Buffer = CreateArray(persistantHandle, wchar_t, screenWidth*screenHeight);	// used for text
	screen->Attribs = CreateArray(persistantHandle, WORD, screenWidth*screenHeight);		// used to color text
	screen->DepthBuffer = CreateArray(transientHandle, float, screenWidth);
	screen->consoleHandle = consoleHandle;
	renderer.SetScreenBuffer(screen);

}

void ConsoleApp::Run(BaseGame* game)
{
	if (game)
	{
		game->SetRenderer(renderer);
		game->LoadContent();

		float deltaTime = 1.0f / 60.0f;

		bool Quit = false;
		while (!Quit)
		{
			Quit = game->Update(deltaTime);
		}
		game->UnloadContent();
	}
}

void ConsoleApp::CleanUp()
{
	CloseHandle(consoleHandle);
	Memory::FreeMemory();
}