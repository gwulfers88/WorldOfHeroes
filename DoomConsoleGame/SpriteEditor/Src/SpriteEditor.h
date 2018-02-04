
#ifndef SPRITE_EDITOR_H
#define SPRITE_EDITOR_H

#include "BaseGame.h"
#include "memory.h"

class SpriteEditor : public BaseGame
{
public:
	SpriteEditor() {}
	virtual ~SpriteEditor() {}

	void SetMemoryHandles(MemoryHandle perm, MemoryHandle trans) { persistantHandle = perm; transientHandle = trans; }

	virtual void LoadContent() override;
	virtual bool Update(float deltaTime) override;
	virtual void UnloadContent() override;

	void SetFilename(char *fname, bool load = false) { filename = fname; loadFile = load; }
	void SetImageSize(int W = 100, int H = 100) { ImgWidth = W; ImgHeight = H; };

protected:
	MemoryHandle persistantHandle;
	MemoryHandle transientHandle;

	bool loadFile;
	char *filename;
	int ImgWidth, ImgHeight;

	int CanvasX;
	int CanvasY;

	float CursorX;
	float CursorY;
	float CursorSpeed;
	float CursorRadius;

	int MaxColors;
	float colorID;
	int ColorsX;
	int ColorsY;
};
#endif
