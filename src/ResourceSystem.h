#ifndef __RESOURCESYSTEM_H__
#define __RESOURCESYSTEM_H__

#include "common/HashTable.h"

class Texture;
class Mesh;
class Shader;
class Material;
class Image;

typedef bool(*loadImageFunc)(const char*, Image*);
struct LoaderPlugin{
	const char* name;
	loadImageFunc pFunc;
};

class ResourceSystem
{
public:
	ResourceSystem();
	~ResourceSystem();

	void SetSearchDir(const char* dir);
	
	Texture* FindTexture(const char* file);
	Material* FindMaterial(const char* file);

	Texture* AddText(const char* text);
	Mesh* AddMesh(const char* file);
	Shader* AddShaderFromFile(const char* vfile, const char* ffile);
	Texture* AddCubeTexture(const char* px, const char* nx, const char* py, const char* ny, const char* pz, const char* nz);
private:
	HashTable _textures;
	HashTable _materials;

	lfStr _searchDir;
};

#endif // !__RESOURCESYSTEM_H__
