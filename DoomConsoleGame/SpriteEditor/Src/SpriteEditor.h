
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

protected:
	MemoryHandle persistantHandle;
	MemoryHandle transientHandle;

	int CanvasX;
	int CanvasY;

	float CursorX;
	float CursorY;
	float CursorSpeed;
	float CursorRadius;

	unsigned short *Colors;
	int MaxColors;
	float colorID;
	int ColorsX;
	int ColorsY;
};
#endif
