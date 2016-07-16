#ifndef __PIPELINEP_H__
#define __PIPELINEP_H__

#include "Pipeline.h"

class PipelineP : public Pipeline
{
public:
	PipelineP();
	virtual ~PipelineP();

	virtual void Init();

	virtual void Begin();
	virtual void Render(drawSurf_t* drawSurf);
	virtual void End();
	virtual void TestrRender();

private:
	GLuint mProgram;

	GLuint mWvp;
};

#endif
