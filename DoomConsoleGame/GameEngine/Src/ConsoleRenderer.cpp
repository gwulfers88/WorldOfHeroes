#include "ConsoleRenderer.h"
#include "memory.h"

ConsoleRenderer::ConsoleRenderer() {}
ConsoleRenderer::~ConsoleRenderer() {}

ScreenBuffer* ConsoleRenderer::GetRenderBuffers() { return screen; }

// This function will build the world hash map.
void ConsoleRenderer::BuildWorldHash()
{
	u32 totalHashCount = EntityManager::EntityCount() * 2;
	worldHash = CreateArray(Memory::GetPersistantHandle(), Entity, totalHashCount);

	worldHashCount = totalHashCount;

	// Loop through all of the entities and add them to the hash table (ONLY THINGS THAT DONT MOVE!!!)
	for (u32 EntityIndex = 0; EntityIndex < EntityManager::EntityCount(); EntityIndex++)
	{
		Entity* entity = EntityManager::GetEntity(EntityIndex);
		if (entity->type == Entity_Wall || entity->type == Entity_Pillar)
		{
			AddEntityToHash(entity);
		}
	}
}

void ConsoleRenderer::AddEntityToHash(Entity* entity)
{
	u32 Hash = HashFunction(entity->position);
	Entity* EntitySlot = worldHash + Hash;
	while (EntitySlot->type != 0)
	{
		Hash++;
		Hash = Hash % worldHashCount;
		EntitySlot = worldHash + Hash;
	}
	worldHash[Hash] = *entity;
}

Entity* ConsoleRenderer::GetEntityFromHash(vec2 pos)
{
	u32 Hash = HashFunction(pos);
	Entity* entity = worldHash + Hash;
	return entity;
}

u32 ConsoleRenderer::HashFunction(vec2 pos)
{
	u32 x = RoundReal32ToUInt32(pos.x);
	u32 y = RoundReal32ToUInt32(pos.y);

	u32 bit = ((x << 0) | (y << 16));
	
	u32 Hash = bit % worldHashCount;
	return Hash;
}

void ConsoleRenderer::SetScreenBuffer(ScreenBuffer *_screen)
{
	screen = _screen;
	Assert(screen);
	// Check to see if we were able to allocate memory from our pool.
	Assert(screen->Buffer);
	Assert(screen->Attribs);
	Assert(screen->DepthBuffer);
}

// Draws a single pixel onto the screen buffer using the desired x, y position, type of pixel and color.
void ConsoleRenderer::DrawPixel(vec2 pos, wchar_t pixel, u16 color)
{
	if (pos.x < 0)
	{
		pos.x = 0;
	}
	if (pos.x >= screen->Width)
	{
		pos.x = screen->Width - 1;
	}
	if (pos.y < 0)
	{
		pos.y = 0;
	}
	if (pos.y >= screen->Height)
	{
		pos.y = screen->Height - 1;
	}

	screen->Buffer[(int)pos.y * screen->Width + (int)pos.x] = pixel;
	screen->Attribs[(int)pos.y * screen->Width + (int)pos.x] = color;
}

void ConsoleRenderer::DrawRect(vec2 pos, vec2 dims, wchar_t pixel, u16 color)
{
	for (u32 row = pos.y; row < dims.y; row++)
	{
		for (u32 col = pos.x; col < dims.x; col++)
		{
			DrawPixel({(r32)col, (r32)row}, pixel, color);
		}
	}
}

// Draws a sprite onto the screen buffer using desired x, y pos and color.
void ConsoleRenderer::DrawSprite(vec2 pos, vec2 dims, wchar_t* spriteData, u16 *colorData)
{
	for (i32 col = 0; col < dims.y; ++col)
	{
		for (i32 row = 0; row < dims.x; ++row)
		{
			i32 pixelX = pos.x + row;
			i32 pixelY = pos.y + col;
			wchar_t glyph = spriteData[col * (int)dims.x + row];
			u16 color = colorData[col * (int)dims.x + row];
			DrawPixel({(r32)pixelX, (r32)pixelY}, glyph, color);
		}
	}
}

// This function prints strings using our font.
void ConsoleRenderer::DrawString(char* text, u32 textCount, Sprite* font, u32 fontCount, vec2 pos, vec2 dims)
{
	u32 row = 0;
	for (int i = 0; i < textCount; i++)
	{
		char c = text[i];	// get the current character of the text
		int index = -1;		// set the index to an invalid index

		if (c >= 'a' && c <= 'z')	// Check for letters with in the text
		{
			index = c - 'a';	// the index is the letter number - 'a' ex; 'a' - 'a' = 0;
		}
		else if (c >= '0' && c <= '9')	// check for numbers in the text
		{
			index = c - '0';	// The index is the character - '0' ex: 1 - 0 = 1;
			index += 26;		// offset the index by the number of letters in the alphabet to get to the numbers
		}

		if (index >= 0 && index < fontCount)
		{
			u32 x = (i * dims.x);	// offset the position of the letter by the width of the fonts
			u32 y = (row * dims.y);	// offset the position of the letter by the height of the fonts
			DrawUI(pos + Vec2(x, y), dims, font + index); // Draw the current font letter or number to the screen
		}
	}
}

// Projects 2D object into 3D World.
void ConsoleRenderer::ProjectObject(Camera* camera, vec2 objP, Sprite* img)
{
	// drawing Objects
	// We want to calculate the distance between the object and the player
	vec2 directionToObj = objP - camera->Position;
	float distanceToPlayer = Length(directionToObj);

	// Then we want to create our forward vector
	// Calculate the objects angle between the forward vector and direction that the object is in.
	vec2 eye = {};
	eye.x = sinf(camera->rotation);
	eye.y = cosf(camera->rotation);
	float objectAngle = atan2f(eye.y, eye.x) - atan2f(directionToObj.y, directionToObj.x);
	if (objectAngle < -PI)
		objectAngle += 2.0f * PI;
	if (objectAngle > PI)
		objectAngle -= 2.0f * PI;

	// Here we check to see if the angle is with in the view.
	bool ObjectInView = fabs(objectAngle) < camera->FOV / 2.0f;

	// If the object is in view and the distance from the object to the player is with in a certain range
	if (ObjectInView && distanceToPlayer >= 0.5f && distanceToPlayer < camera->Depth)
	{
		// Then we want to calculate the objects dimensions in world space.
		// First we calculate where the objects ceiling starts based on the distance
		// Then the floor location
		// this will give us how tall the object will look with in our view. The farther the smaller it will be. The closer the bigger it will be.
		float ObjectCeiling = (float)(screen->Height / 2.0f) - screen->Height / distanceToPlayer;
		float ObjectFloor = screen->Height - ObjectCeiling;
		float ObjectHeight = ObjectFloor - ObjectCeiling;
		float ObjectAspectRatio = (float)img->Height / (float)img->Width;
		float ObjectWidth = ObjectHeight / ObjectAspectRatio;
		float ObjectCenter = (0.5f * (objectAngle / (camera->FOV / 2.0f)) + 0.5f) * (float)screen->Width;

		for (int y = 0; y < ObjectHeight; ++y)
		{
			for (int x = 0; x < ObjectWidth; ++x)
			{
				vec2 sample = Vec2(x / ObjectWidth, y / ObjectHeight);
				int Row = sample.y * img->Height;
				int Col = sample.x * img->Width;
				wchar_t glyph = img->Pixels[Row * img->Width + Col];
				u16 color = img->Colors[Row * img->Width + Col];
				int ObjectCol = RoundReal32ToInt32((ObjectCenter + x - (ObjectWidth / 2.0f)));
				if (ObjectCol >= 0 && ObjectCol < screen->Width)
				{
					if (glyph != ' ' && screen->DepthBuffer[ObjectCol] >= distanceToPlayer)
						DrawPixel({ (r32)ObjectCol, (r32)ObjectCeiling + y }, glyph, color);
				}
			}
		}
	}
}

void ConsoleRenderer::ProjectWorld(Camera* camera, wchar_t* Map, u32 MapW, u32 MapH, Sprite* wall)
{
	// TODO: Should we pass the textures for the wall through???
	// TODO: Figure out how to not have collisions when adding into hash
	for (int x = 0; x < screen->Width; ++x)
	{
		// Find the ray angle based on the players Rotattion angle
		//float RayAngle = (playerAngle - FOV / 2) + ((float)x / screenWidth) * FOV;
		float RayAngle = (camera->rotation - camera->FOV / 2) + ((float)x / screen->Width) * camera->FOV;

		float StepSize = 0.1f;			// This is how far we will advanced the ray per iteration
		float DistanceToWall = 0.0f;	// 

		bool HitWall = false;
		bool HitDoor = false;

		vec2 eye = {};
		eye.x = sinf(RayAngle);
		eye.y = cosf(RayAngle);

		r32 sampleX = 0;

		short Color = PIXEL_COLOR_WHITE;

		while (!HitWall && !HitDoor && DistanceToWall < camera->Depth)
		{
			DistanceToWall += StepSize;

			r32 TestX = (camera->Position.x + eye.x * DistanceToWall);
			r32 TestY = (camera->Position.y + eye.y * DistanceToWall);

			vec2 deltaP = camera->Position + eye * DistanceToWall;

			// Our ray has gone Out of bounds
			if (TestX < 0 || TestX >= MapW || TestY < 0 || TestY >= MapH)
			{
				HitWall = true;
				DistanceToWall = camera->Depth;
			}
			else
			{
				// Ray is still in bounds to test cell for walls
				if (Map[(i32)TestY * MapW + (i32)TestX] == L'#')
				{
					// Ray has hit a wall
					HitWall = true;

					// Calculate the Texture coordinates here.
					Color = PIXEL_COLOR_DARK_CYAN;

					vec2 wallMidP = Vec2(((u32)TestX) + 0.5f, ((u32)TestY) + 0.5f);
					vec2 testP = camera->Position + eye * DistanceToWall;
					float testAngle = atan2f(testP.y - wallMidP.y, testP.x - wallMidP.x);

					if (testAngle >= -PI * 0.25f && testAngle < PI * 0.25f)
						sampleX = testP.y - TestY;
					if (testAngle >= PI * 0.25f && testAngle < PI * 0.75f)
						sampleX = testP.x - TestX;
					if (testAngle < -PI * 0.25f && testAngle >= -PI * 0.75f)
						sampleX = testP.x - TestX;
					if (testAngle >= PI * 0.75f || testAngle < -PI * 0.75f)
						sampleX = testP.y - TestY;
				}
				// Ray is still in bounds to test cell for Doors
				else if (Map[(i32)TestY * MapW + (i32)TestX] == L'D')
				{
					vec2 doorMidP = Vec2(((u32)TestX) + 0.5f, ((u32)TestY) + 0.5f);

					if (deltaP.x > doorMidP.x - 0.05f && deltaP.x < doorMidP.x + 0.05f)
					{
						// Calculate the Texture coordinates here.
						Color = PIXEL_COLOR_DARK_MAGENTA;

						vec2 testP = camera->Position + eye * DistanceToWall;
						float testAngle = atan2f(testP.y - doorMidP.y, testP.x - doorMidP.x);

						if (testAngle >= -PI * 0.25f && testAngle < PI * 0.25f)
							sampleX = testP.y - TestY;
						if (testAngle >= PI * 0.25f && testAngle < PI * 0.75f)
							sampleX = testP.x - TestX;
						if (testAngle < -PI * 0.25f && testAngle >= -PI * 0.75f)
							sampleX = testP.x - TestX;
						if (testAngle >= PI * 0.75f || testAngle < -PI * 0.75f)
							sampleX = testP.y - TestY;

						// Ray has hit a wall
						HitDoor = true;
					}
				}
			}
		}

		// Calculate the distance to the ceiling
		int DistanceToCeiling = (((float)screen->Height * 0.5f) - ((float)screen->Height / DistanceToWall));
		int DistanceToFloor = (screen->Height - DistanceToCeiling);

		screen->DepthBuffer[x] = DistanceToWall;

		// Shade walls based on distance 
		wchar_t shade = PIXEL_SOLID;

		if (DistanceToWall <= camera->Depth / 16)
		{
			shade = PIXEL_SOLID;
		}
		else if (DistanceToWall < camera->Depth / 8)
		{
			shade = PIXEL_SEMI_DARK;
		}
		else if (DistanceToWall < camera->Depth / 4)
		{
			shade = PIXEL_MEDIUM_DARK;
		}
		else if (DistanceToWall < camera->Depth / 2)
		{
			shade = PIXEL_DARK;
		}
		else
			shade = ' ';

		for (int y = 0; y < screen->Height; ++y)
		{
			if (y <= DistanceToCeiling)
			{
				DrawPixel({ (r32)x, (r32)y }, PIXEL_SOLID, PIXEL_COLOR_BLACK);
			}
			else if (y > DistanceToCeiling && y <= DistanceToFloor)	// Wall
			{
				if (HitWall)
				{
					r32 sampleY = ((r32)y - DistanceToCeiling) / (DistanceToFloor - DistanceToCeiling);
					Color = SampleSprite(Vec2(sampleX, sampleY), wall).Color;
					DrawPixel({ (r32)x, (r32)y }, shade, Color);
				}
				if (HitDoor)
				{
					DrawPixel({ (r32)x, (r32)y }, shade, Color);
				}
			}
			else
			{
				// Shade floor based on distance
				float b = 1.0f - (((float)y - screen->Height * 0.5f) / ((float)screen->Height * 0.5f));
				if (b < 0.25)
				{
					shade = PIXEL_SOLID;
					Color = PIXEL_COLOR_DARK_GREEN;
				}
				else if (b < 0.5)
				{
					shade = PIXEL_SEMI_DARK;
					Color = PIXEL_COLOR_DARK_GREEN;
				}
				else if (b < 0.75)
				{
					shade = PIXEL_MEDIUM_DARK;
					Color = PIXEL_COLOR_DARK_GREEN;
				}
				else if (b < 0.9)
				{
					shade = PIXEL_DARK;
					Color = PIXEL_COLOR_DARK_GREEN;
				}
				DrawPixel({ (r32)x, (r32)y }, shade, Color);
			}
		}
	}
}

// Draws a sprite with the specified dimensions
void ConsoleRenderer::DrawUI(vec2 pos, vec2 dims, Sprite* img)
{
	for (i32 y = 0; y < dims.y; y++)
	{
		for (i32 x = 0; x < dims.x; x++)
		{
			// Calculate the texel coordinate for the rectangle
			vec2 sample = Vec2(x / dims.x, y / dims.y);
			// Sample the texture you want to use using the texel coords.
			SpriteSample result = SampleSprite(sample, img);

			// Draw Pixel
			if(result.Pixel != L' ')
				DrawPixel({ pos.x + x, pos.y + y }, PIXEL_SOLID, result.Color);
		}
	}
}

// Clears the entire screen buffer to the desired color.
void ConsoleRenderer::ClearBuffer(u16 color)
{
	for (i32 y = 0; y < screen->Height; ++y)
	{
		for (i32 x = 0; x < screen->Width; ++x)
		{
			DrawPixel({(r32)x, (r32)y}, PIXEL_SOLID, color);
		}
	}
}

// Draws the entire buffer to the screen
void ConsoleRenderer::PresentBuffer()
{
	DWORD bytesWritten = 0;
	screen->Buffer[screen->Width * screen->Height - 1] = '\0';
	WriteConsoleOutputCharacter(screen->consoleHandle, screen->Buffer, screen->Width * screen->Height, { 0, 0 }, &bytesWritten);
	WriteConsoleOutputAttribute(screen->consoleHandle, screen->Attribs, screen->Width * screen->Height, { 0, 0 }, &bytesWritten);
}