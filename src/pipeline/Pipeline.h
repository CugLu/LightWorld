#ifndef __PIPELINE_H__
#define	__PIPELINE_H__

#include "r_public.h"

class Pipeline
{
public:
	virtual ~Pipeline() {}

	virtual void Init() = 0;

	virtual void Begin() = 0;
	virtual void Render(drawSurf_t* drawSurf) = 0;
	virtual void End() = 0;
	virtual void TestrRender() = 0;
};

#endif
