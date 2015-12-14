#ifndef __LFSPINE_H__
#define __LFSPINE_H__

#include "Shape.h"
#include "spine/spine.h"

class lfSpine : public Render
{
public:
	lfSpine();
	~lfSpine();

	void SetFile(const char* skeletonDataFile, const char* atlasFile);

	void Update (float deltaTime);
private:
	spAtlas* _atlas;
	spSkeleton* _skeleton;
};


#endif


