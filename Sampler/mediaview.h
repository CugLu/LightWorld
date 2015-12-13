#ifndef __SAMPLER1_H__
#define __SAMPLER1_H__

#include "Game.h"
#include "common/Vec3.h"

class ShadowSampler : public Game
{
public:
	ShadowSampler( void );
	~ShadowSampler();

	void Init();

	void Frame();

	void ProcessEvent(sysEvent_s* event);

	void ShowNext();
private:
	Sprite* _defaultSprite;
};



#endif


