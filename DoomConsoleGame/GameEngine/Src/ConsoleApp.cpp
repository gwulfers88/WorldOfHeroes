#include "ConsoleApp.h"

void ConsoleApp::CreateConsole(size_t memorySize, int width, int height, int fontw, int fonth)
{
	screenWidth = width;
	screenHeight = height;
	
	// Initialize our memory pools.
	Memory::InitializeMemory(memorySize);

	persistantHandle = Memory::InitializeChunk((u32)(memorySize * 0.5f));
	transientHandle = Memory::InitializeChunk((u32)(memorySize * 0.5f));

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
		screenWidth = consoleInfo.dwMaximumWindowSize.X;
		goto retry;
	}
	if (screenHeight > consoleInfo.dwMaximumWindowSize.Y)
	{
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
		HWND window = GetConsoleWindow();
		Assert(window);
		HDC dc = GetDC(window);
		Assert(dc);

		u32 RefreshRate = GetDeviceCaps(dc, VREFRESH);
		if (RefreshRate < 30)
		{
			RefreshRate = 30;
		}
		r32 deltaTime = (1.0f / RefreshRate);
		
		// NOTE(casey): Set the Windows scheduler granularity to 1ms
		// so that our Sleep() can be more granular.
		UINT DesiredSchedulerMS = 1;
		bool SleepIsGranular = (timeBeginPeriod(DesiredSchedulerMS) == TIMERR_NOERROR);

		LARGE_INTEGER PerformaceFrequency = {};
		QueryPerformanceFrequency(&PerformaceFrequency);
		GlobalPerforceFreq = PerformaceFrequency.QuadPart;

		u64 StartCounter = win32_GetClockTime();

		game->SetRenderer(renderer);
		game->LoadContent();

		bool Quit = false;
		while (!Quit)
		{
			u64 UpdateCounter = win32_GetClockTime();
			// Handle Mouse Input - Check for window events
			INPUT_RECORD inBuf[32];
			DWORD events = 0;
			GetNumberOfConsoleInputEvents(consoleHandle, &events);
			if (events > 0)
				ReadConsoleInput(consoleHandle, inBuf, events, &events);

			// Handle events - we only care about mouse clicks and movement
			// for now
			for (DWORD i = 0; i < events; i++)
			{
				switch (inBuf[i].EventType)
				{
				case FOCUS_EVENT:
				{
					//m_bConsoleInFocus = inBuf[i].Event.FocusEvent.bSetFocus;
				}
				break;

				case MOUSE_EVENT:
				{
					switch (inBuf[i].Event.MouseEvent.dwEventFlags)
					{
					case MOUSE_MOVED:
					{
						//m_mousePosX = inBuf[i].Event.MouseEvent.dwMousePosition.X;
						//m_mousePosY = inBuf[i].Event.MouseEvent.dwMousePosition.Y;
					}
					break;

					case 0:
					{
						//for (int m = 0; m < 5; m++)
							//m_mouseNewState[m] = (inBuf[i].Event.MouseEvent.dwButtonState & (1 << m)) > 0;

					}
					break;

					default:
						break;
					}
				}
				break;

				default:
					break;
					// We don't care just at the moment
				}
			}


			game_controller Controller = {};

			// Update key information
			for (u32 key = 0; key < 256; key++)
			{
				if (key == 'W')
				{
					if ((GetAsyncKeyState(key) & 0x8000))
					{
						Controller.MoveUp.pressed = true;
					}
					else
					{
						Controller.MoveUp.pressed = false;
					}
				}
				if (key == 'A')
				{
					if ((GetAsyncKeyState(key) & 0x8000))
					{
						Controller.MoveLeft.pressed = true;
					}
					else
					{
						Controller.MoveLeft.pressed = false;
					}
				}
				if (key == 'S')
				{
					if ((GetAsyncKeyState(key) & 0x8000))
					{
						Controller.MoveDown.pressed = true;
					}
					else
					{
						Controller.MoveDown.pressed = false;
					}
				}
				if (key == 'D')
				{
					if ((GetAsyncKeyState(key) & 0x8000))
					{
						Controller.MoveRight.pressed = true;
					}
					else
					{
						Controller.MoveRight.pressed = false;
					}
				}
				if (key == 'Q')
				{
					if ((GetAsyncKeyState(key) & 0x8000))
					{
						Controller.ActionLeft.pressed = true;
					}
					else
					{
						Controller.ActionLeft.pressed = false;
					}
				}
				if (key == 'E')
				{
					if ((GetAsyncKeyState(key) & 0x8000))
					{
						Controller.ActionRight.pressed = true;
					}
					else
					{
						Controller.ActionRight.pressed = false;
					}
				}
				if (key == VK_UP)
				{
					if ((GetAsyncKeyState(key) & 0x8000))
					{
						Controller.ActionUp.pressed = true;
					}
					else
					{
						Controller.ActionUp.pressed = false;
					}
				}
				if (key == VK_DOWN)
				{
					if ((GetAsyncKeyState(key) & 0x8000))
					{
						Controller.ActionDown.pressed = true;
					}
					else
					{
						Controller.ActionDown.pressed = false;
					}
				}
				if (key == VK_SPACE)
				{
					if ((GetAsyncKeyState(key) & 0x8000))
					{
						Controller.RightTrigger.pressed = true;
					}
					else
					{
						Controller.RightTrigger.pressed = false;
					}
				}
			}

			game->SetController(&Controller);
			Quit = game->Update(deltaTime);
			r32 UpdateSecondsElapsed = (r32)win32_GetSecondsElapsed(StartCounter, UpdateCounter) * 1000.0f;

			u64 WorkCounter = win32_GetClockTime();
			r32 WorkSecondsElapsed = (r32)win32_GetSecondsElapsed(StartCounter, WorkCounter);

			// TODO(casey): NOT TESTED YET!  PROBABLY BUGGY!!!!!
			r32 SecondsElapsedForFrame = WorkSecondsElapsed;
			if (SecondsElapsedForFrame < deltaTime)
			{
				if (SleepIsGranular)
				{
					DWORD SleepMS = (DWORD)(1000.0f * (deltaTime -
						SecondsElapsedForFrame));
					if (SleepMS > 0)
					{
						Sleep(SleepMS);
					}
				}

				r32 TestSecondsElapsedForFrame = (r32)win32_GetSecondsElapsed(StartCounter, win32_GetClockTime());
				if (TestSecondsElapsedForFrame < deltaTime)
				{
					// TODO(casey): LOG MISSED SLEEP HERE
				}

				while (SecondsElapsedForFrame < deltaTime)
				{
					SecondsElapsedForFrame = (r32)win32_GetSecondsElapsed(StartCounter, win32_GetClockTime());
				}
			}
			else
			{
				// TODO(casey): MISSED FRAME RATE!
				// TODO(casey): Logging
			}

			u64 EndCounter = win32_GetClockTime();
			r32 msPerFrame = (r32)win32_GetSecondsElapsed(StartCounter, EndCounter)*1000.0f;
			r32 framesPerSecond = 1.0f / (r32)win32_GetSecondsElapsed(StartCounter, EndCounter);

			wchar_t titleFmt[] = L"Console Game Engine - %.02fms/f %.02ffps - update loop: %.02fms/f";
			wchar_t buffer[256];
			_swprintf_c(buffer, ArrayCount(titleFmt), titleFmt, msPerFrame, framesPerSecond, UpdateSecondsElapsed);
			SetConsoleTitleW(buffer);

			StartCounter = EndCounter;

		}
		game->UnloadContent();
	}
}

void ConsoleApp::CleanUp()
{
	CloseHandle(consoleHandle);
	Memory::FreeMemory();
}