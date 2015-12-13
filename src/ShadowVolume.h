#pragma once
#include "common/Vec3.h"
#include "common/Array.h"

class ShadowVolume
{
public:
	ShadowVolume(Vec3* vertices, unsigned short* indices, unsigned int faceCount);
	~ShadowVolume();
	
	void createVolumeShadow();
	int  createEdgesAndCaps(Vec3 lightPosition, Array<Vec3>* svp);
	void createSideFace();
	void calculateAdjacency();

	Vec3 mCapsVertices;
	Vec3 mEdgeVertices;


	Array<Vec3> mShadowVolume; 
	Array<unsigned short> mEdges;
	bool*			mIsFrontFace;
	int*			mAdjacency;
	Vec3*			mVertices;
	unsigned short* mIndices;
	unsigned int	mFaceCount;
};

