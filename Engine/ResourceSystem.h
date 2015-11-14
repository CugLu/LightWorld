#ifndef __RESOURCESYSTEM_H__
#define __RESOURCESYSTEM_H__

#include "common/hashtable.h"

class Texture;
class StaticModel;
class Shader;
class Material;
class Image;

#define MAX_SHADER_COUNT 32

typedef Shader* (*LoadShaderFunc)();
struct ShaderPlugin
{
	int name;
	LoadShaderFunc func;
};

typedef bool(*loadImageFunc)(const char*, Image&);
struct LoaderPlugin{
	const char* name;
	loadImageFunc pFunc;
};

class ResourceSystem
{
public:
	ResourceSystem();
	~ResourceSystem();

	bool LoadAllShader();

	Texture* AddTexture(const char* file);

	Texture* AddText(const char* text);

	StaticModel* AddMesh(const char* file);

	Shader* FindShader(int shaderId);

	Shader* AddShaderFromFile(const char* vfile, const char* ffile);

	Material* AddMaterial(const char* file);
private:

	hashtable _textures;

	hashtable _materials;

	Shader* _shaders[MAX_SHADER_COUNT];
};

extern ResourceSystem* resourceSys;
#endif // !__RESOURCESYSTEM_H__
