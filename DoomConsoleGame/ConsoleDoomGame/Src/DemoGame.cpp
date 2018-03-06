#include "DemoGame.h"
#include "platform.h"
#include "File.h"

struct Entity
{
	vec2 p;
	vec2 dims;
	vec2 forward;
	float rotation;
};

Entity player = {};

void DemoGame::LoadContent()
{
	player.rotation = 0;
	
	mapW = 40;
	mapH = 20;

	map =
	{
		L"########################################"
		L"#..........D.....D.....................#"
		L"#..........#######...#######D#######...#"
		L"#..........#.....#...#.............#...#"
		L"#......S...#.....#...######...######...#"
		L"############.....#...#.............#...#"
		L".................#...#.............#...#"
		L".................#...############..#...#"
		L".................#...#.............#...#"
		L".................D...#.............#...#"
		L".................D...#..############...#"
		L".................#...#.............#...#"
		L".................#...#.............#...#"
		L".................#...############..#...#"
		L".................#...#.............#...#"
		L".................#...#.............#...#"
		L".................#...####D##########...#"
		L".................#.....................#"
		L".................#.....................#"
		L".................#######################"
	};

	// Load Fonts
	char c = 'a';
	for (int i = 0; i < ArrayCount(font); i++)
	{
		char fid[2] = { c, 0 };
		char buffer[256] = {};
		strcat(buffer, "data/fonts/font_");
		strcat(buffer, fid);
		strcat(buffer, ".sprt");

		LoadSprite(buffer, font + i);
		if (c == 'z')
		{
			c = '0';
			continue;
		}
		c++;
	}
	fontDims = Vec2(8, 8);

	// Load the pillar texture
	LoadSprite("data/pillar_01.sprt", &pillar);
	pillarP = Vec2(27.5f, 5.5f);
	
	// Load the wall texture
	LoadSprite("data/wall_01.sprt", &wall);

	// Load the hud background texture
	LoadSprite("data/hud_bg.sprt", &hudBG);
	i32 w = RoundReal32ToInt32(renderer.GetRenderBuffers()->Width / 7.0f);
	hudDims = Vec2((r32)w, 20);
	hudP = Vec2(0, (r32)renderer.GetRenderBuffers()->Height - hudDims.y);

	// Setup player
	for (int y = 0; y < mapH; ++y)
	{
		for (int x = 0; x < mapW; ++x)
		{
			if (map[y * mapW + x] == 'S')
			{
				player.p = Vec2((r32)x, (r32)y);
				player.dims = Vec2(0.5f, 0.5f);
			}
		}
	}
}

bool DemoGame::Update(float deltaTime)
{
	bool Quit = false;
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		Quit = true;

	float playerSpeed = 10.0f;
	vec2 direction = {}; // X: Fwd/Bkwd, Y: StrafeL, StrafeR

	if (Controller)
	{
		// Forward / Backward Movement
		if (Controller->MoveUp.pressed)
		{
			direction.x = 1;
		}
		else if (Controller->MoveDown.pressed)
		{
			direction.x = -1;
		}

		// Roatting the Angle CCW / CW
		if (Controller->MoveLeft.pressed)
		{
			player.rotation -= (playerSpeed * 0.75f) * deltaTime;
		}
		else if (Controller->MoveRight.pressed)
		{
			player.rotation += (playerSpeed * 0.75f) * deltaTime;
		}

		player.forward = Normalize(Vec2(sinf(player.rotation), cosf(player.rotation)));

		// Straffe Movement
		if (Controller->ActionLeft.pressed)
		{
			direction.y = -1;
		}
		else if (Controller->ActionRight.pressed)
		{
			direction.y = 1;
		}

	}

	// Calculate the right vector based on the Up and the Forward Vector of the player
	vec3 up = Vec3(0, 0, 1);
	vec3 right = Cross(Vec3(player.forward, 0), up);
	player.p += ((player.forward * direction.x) + (right.xy * direction.y)) * playerSpeed * deltaTime;
	
	// Physics Simulation
	// Collision between the world (WALLS) and the player
	// Below is collision between pillar obj and the player
	vec2 pillarDims = Vec2(0.25f, 0.25f) + player.dims;
	vec2 minPillarP = pillarP - pillarDims;
	vec2 maxPillarP = pillarP + pillarDims;

	if (player.p.x > minPillarP.x &&
		player.p.x < maxPillarP.x &&
		player.p.y > minPillarP.y &&
		player.p.y < maxPillarP.y ||
		map[(int)player.p.y * mapW + (int)player.p.x] == L'#')
	{
		// Collision
		player.p -= ((player.forward * direction.x) + (right.xy * direction.y)) * playerSpeed * deltaTime;
	}

	// Clear buffer to certain color
	renderer.ClearBuffer(PIXEL_COLOR_GREY);

	// Ray Casting Routine
	int screenWidth = renderer.GetRenderBuffers()->Width;
	int screenHeight = renderer.GetRenderBuffers()->Height;

	Camera camera = {};
	camera.Position = player.p;
	camera.rotation = player.rotation;
	camera.FOV = PI / 4.0f;
	camera.Depth = 40.0f;
	
	// Projecting 2D to 3D world
	renderer.ProjectWorld(&camera, map, mapW, mapH, &wall);
	renderer.ProjectObject(&camera, pillarP, &pillar);

	// HUD Routines Below
	// 2D Map
	vec2 screenOnePos = Vec2(10, 10);
	
	// Draw Map
	for (int y = 0; y < mapH; ++y)
	{
		for (int x = 0; x < mapW; ++x)
		{
			vec2 p = Vec2(x, y);
			if (map[y * mapW + x] == '#')
			{
				renderer.DrawPixel(screenOnePos + p, PIXEL_SOLID, PIXEL_COLOR_DARK_RED); // Walls
			}
			else
				renderer.DrawPixel(screenOnePos + p, PIXEL_SOLID, PIXEL_COLOR_GREY); // Floor
		}
	}

	// This will draw the players field of view on the 2D map.
	for (int x = 0; x < mapW; ++x)
	{
		float ViewAngle = (camera.rotation - camera.FOV / 2) + ((float)x / mapW) * camera.FOV;
		vec2 eye = {};
		eye.x = sinf(ViewAngle);
		eye.y = cosf(ViewAngle);

		for (int y = 0; y < mapH; ++y)
		{
			renderer.DrawPixel(camera.Position + screenOnePos + eye * y, PIXEL_SOLID, PIXEL_COLOR_LIGHT_BLUE);
		}
	}

	// Draw player
	renderer.DrawPixel(player.p + screenOnePos, PIXEL_SOLID, PIXEL_COLOR_LIGHT_GREEN);

	// Draw Pillar
	renderer.DrawPixel(pillarP + screenOnePos, PIXEL_SEMI_DARK, PIXEL_COLOR_LIGHT_RED);

	// Drawing UI The dumb way
	for (int i = 0; i < 7; i++)
	{
		vec2 bg_offset = Vec2(i * (hudDims.x), 0);
		vec2 text_offset = Vec2(i * (hudDims.x), 10);
		renderer.DrawUI(hudP + bg_offset, hudDims, &hudBG);
		if (i == 0)
		{
			renderer.DrawString("0", ArrayCount("0"), font, ArrayCount(font), hudP + bg_offset + Vec2(10, 0), fontDims);
			u32 strW = ArrayCount("ammo") * fontDims.x;
			u32 strHalf = RoundReal32ToUInt32(strW * 0.5f);
			renderer.DrawString("ammo", ArrayCount("ammo"), font, ArrayCount(font), hudP + text_offset, fontDims);
		}
		else if (i == 1)
		{
			renderer.DrawString("100", ArrayCount("100"), font, ArrayCount(font), hudP + bg_offset + Vec2(10, 0), fontDims);
			u32 strW = ArrayCount("health") * fontDims.x;
			u32 strHalf = RoundReal32ToUInt32(strW * 0.5f);
			renderer.DrawString("health", ArrayCount("health"), font, ArrayCount(font), hudP + text_offset, fontDims);
		}
		else if (i == 2)
		{
			u32 strW = ArrayCount("arms") * fontDims.x;
			u32 strHalf = RoundReal32ToUInt32(strW * 0.5f);
			renderer.DrawString("arms", ArrayCount("arms"), font, ArrayCount(font), hudP + text_offset, fontDims);
		}
		else if (i == 4)
		{
			renderer.DrawString("100", ArrayCount("100"), font, ArrayCount(font), hudP + bg_offset + Vec2(10, 0), fontDims);
			u32 strW = ArrayCount("armor") * fontDims.x;
			u32 strHalf = RoundReal32ToUInt32(strW * 0.5f);
			renderer.DrawString("armor", ArrayCount("armor"), font, ArrayCount(font), hudP + text_offset, fontDims);
		}
	}

	// Present buffers to the screen
	renderer.PresentBuffer();

	return Quit;
}

void DemoGame::UnloadContent()
{

}