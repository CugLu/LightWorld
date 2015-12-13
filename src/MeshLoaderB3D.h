/*
http://www.blitzbasic.com/sdkspecs/sdkspecs/b3dfile_specs.txt
*/

#ifndef __MESHLOADB3D_H__
#define __MESHLOADB3D_H__

#include "common/String.h"
#include "common/Quat.h"
#include "common/Vec3.h"
#include "common/Vec2.h"
#include "common/Array.h"

#include "r_public.h"

class lfFile;
class Joint;
class Mesh;

class MeshLoaderB3D
{
public:
	MeshLoaderB3D();
	~MeshLoaderB3D();

	struct SB3dChunkHeader
	{
		char name[4];
		int size;
	};

	struct SB3dTexture
	{
		lfStr TextureName;
		int Flags;
		int Blend;
		float Xpos;
		float Ypos;
		float Xscale;
		float Yscale;
		float Angle;
	};

	struct SB3dMaterial
	{
		SB3dMaterial() : red(1.0f), green(1.0f),
			blue(1.0f), alpha(1.0f), shininess(0.0f), blend(1),
			fx(0)
		{
		//	for (unsigned int i=0; i<3; ++i)
				//Textures[i]=0;
		}
		//video::SMaterial Material;
		float red, green, blue, alpha;
		float shininess;
		int blend,fx;
		//SB3dTexture *Textures[video::MATERIAL_MAX_TEXTURES];
	};

	bool MeshLoaderB3D::Load(const char* file) ;

	void ReadTexs();
	bool ReadVrts();
	Joint* ReadNode();
	void ReadBrus();
	void ReadBone(Joint* joint);
	void ReadMesh();
	void ReadTris();
	void ReadAnim();
	void ReadKey(Joint* joint);

	lfStr ReadChunk();
	bool CheckSize();
	void ExitChunk();

	void printTree(const char *psz, ...);

	long _curpos;
	Array<unsigned int> _stack;
	Array<SB3dTexture> _textures;
	Array<unsigned short> _indices;

	lfFile*  _file;
	Mesh* _mesh;
};

#endif
