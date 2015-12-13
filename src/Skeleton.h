//#pragma once
//#include "array.h"
//#include "quaternion.h"
//#include <string>
//using std::string;

//struct SPositionKey
//{
//	float frame;
//	Vector3D position;
//};

////! Animation keyframe which describes a new scale
//struct SScaleKey
//{
//	float frame;
//	Vector3D scale;
//};

////! Animation keyframe which describes a new rotation
//struct SRotationKey
//{
//	float frame;
//	Vector3D rotation;
//};

//struct SWeight
//{
//	//! Index of the mesh buffer
//	unsigned short buffer_id; //I doubt 32bits is needed

//	//! Index of the vertex
//	unsigned int vertex_id; //Store global ID here

//	//! Weight Strength/Percentage (0-1)
//	float strength;

//private:
//	//! Internal members used by CSkinnedMesh
//	friend class CSkinnedMesh;
//	bool *Moved;
//	Vector3D StaticPos;
//	Vector3D StaticNormal;
//};


//struct SJoint
//{
//	SJoint() : UseAnimationFrom(0), GlobalSkinningSpace(false),
//			positionHint(-1),scaleHint(-1),rotationHint(-1)
//	{
//	}

//	//! The name of this joint
//	string Name;

//	//! Local matrix of this joint
//	Matrix4x4 LocalMatrix;

//	//! List of child joints
//	core::array<SJoint*> Children;

//	//! List of attached meshes
//	//array<unsigned int> AttachedMeshes;

//	//! Animation keys causing translation change
//	core::array<SPositionKey> PositionKeys;

//	//! Animation keys causing scale change
//	core::array<SScaleKey> ScaleKeys;

//	//! Animation keys causing rotation change
//	core::array<SRotationKey> RotationKeys;

//	//! Skin weights
//	core::array<SWeight> Weights;

//	//! Unnecessary for loaders, will be overwritten on finalize
//    Matrix4x4 GlobalMatrix;
//	Matrix4x4 GlobalAnimatedMatrix;
//	Matrix4x4 LocalAnimatedMatrix;
//	Vector3D Animatedposition;
//	Vector3D Animatedscale;
//	Quaternion Animatedrotation;

//	Matrix4x4 GlobalInversedMatrix; //the x format pre-calculates this

//private:
//	//! Internal members used by CSkinnedMesh
//	friend class CSkinnedMesh;

//	SJoint *UseAnimationFrom;
//	bool GlobalSkinningSpace;

//	int positionHint;
//	int scaleHint;
//	int rotationHint;
//};


//class CSkeleton
//{
//public:
//	CSkeleton();
//	~CSkeleton();
//};

