#ifndef __GAME_H__
#define __GAME_H__
#include "sys/sys_public.h"

class Camera;
class Sprite;
class Mesh;

class RenderSystem;
class ScriptSystem;

class Game
{
public:
	virtual		~Game() {};
	virtual void  Init();
	virtual void  Frame() = 0;

protected:
	RenderSystem* _renderSys;
	ScriptSystem* _scriptSys;

};


extern Game* game;
#endif


