#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "color4.h"
#include "common/Vec3.h"

enum E_LIGHT_TYPE
{
	ELT_POINT,
	ELT_SPOT,
	ELT_DIRECTIONAL,
	ELT_COUNT
};

class Light
{
public:
	Light();
	~Light();

	void GetView();

private:
	Vec3 _positon;
	Vec3 _dir;

	Vec3 _matView;
};

#endif