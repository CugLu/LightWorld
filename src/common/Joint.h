#ifndef __JOINT_H__
#define __JOINT_H__

#include "precompiled.h"
#include "Vec3.h"
#include "Mat4.h"
#include "Quat.h"
#include "String.h"
#include "Array.h"


struct PositionKey
{
	Vec3 position;
	float frame;
};

struct ScaleKey
{
	Vec3 position;
	float frame;
};

struct RotationKey
{
	Quat rotation;
	float frame;
};

class Joint
{
public:
	Joint();
	~Joint();

	lfStr name;
	Vec3 position;
	Vec3 scale;
	Quat rotation;
	Array<Joint*> children;
	Joint* parent;

	Array<PositionKey> positionKeys;
	Array<RotationKey> rotationKeys;
	Array<ScaleKey>    scaleKeys;
	Array<int>         vertexIndices;
	Array<float>       vertexWeights;

	mat4 globalAnimatedMatrix;
	mat4 globalInvMatrix;

	mat4 globalPosition;

	void GetPositionKeyFramesAtFrame(float frame, PositionKey* preKey, PositionKey* nextKey);
	void GetScaleKeyFramesAtFrame(float frame, ScaleKey* preKey, ScaleKey* nextKey);
	void GetRotationKeyFramesAtFrame(float frame, RotationKey* preKey, RotationKey* nextKey);
	void GetFrame(float frame, Vec3& position, Quat& rotation);
};

#endif
