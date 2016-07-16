#include "ResourceSystem.h"
#include "Image.h"
#include "image_load.h"
#include "Texture.h"
#include <algorithm>
#include <iostream>
#include "MeshLoaderB3D.h"
#include "Shader.h"
#include "glutils.h"
#include "sys/sys_public.h"
#include "Mesh.h"
#include "Material.h"
#include "common/File.h"

#include "MeshLoaderLwo.h"
#include "MeshLoader3DS.h"

#include "ShaderSource.h"

static LoaderPlugin loaderPlugin[] = {
    { "jpg", image_load_jpg},
    { "png", image_load_png},
	{ "tga", image_load_tga},
	{ "bmp", image_load_bmp},
};
static int TexPluginCount = sizeof(loaderPlugin) / sizeof(LoaderPlugin);
//--------------------------------------------------------------------------------------------

static sysTextContent_t textContent;

static const char* DEFAULT_TEXTURE_PATH = "../Media/nskinbl.jpg";

ResourceSystem::ResourceSystem() : _searchDir("")
{

}

ResourceSystem::~ResourceSystem()
{
	
}

Texture* ResourceSystem::FindTexture(const char* file)
{
	Texture* texture = NULL;

	void* it = _textures.Get(file);
    if( it != NULL ) {
		texture = (Texture*)it;
		return texture;
    }

	Image image;
	lfStr fullpath = _searchDir + file;
    
	for (int i = 0; i < TexPluginCount; ++i)
	{
		if (fullpath.Find(loaderPlugin[i].name) == -1)
			continue;

		if( !loaderPlugin[i].pFunc(fullpath.c_str(), &image) )
		{
			Sys_Printf( "load image %s failed\n", fullpath.c_str() );
			return FindTexture(DEFAULT_TEXTURE_PATH);
		}

		texture = new Texture();
		texture->Init(&image);

		_textures.Put(file, texture);
		return texture;
	}

	Sys_Printf( "load image %s failed\n", fullpath.c_str() );
	return FindTexture(DEFAULT_TEXTURE_PATH);
};

Mesh* ResourceSystem::AddMesh(const char* file)
{
	lfStr fullpath = _searchDir + file;
	if (fullpath.Find(".lwo") != -1) { 
		unsigned int failId;
		int failedPos;
		lwObject* object = lwGetObject(file, &failId, &failedPos);

		Mesh* mesh = new Mesh;
		mesh->ConvertLWOToModelSurfaces(object);
		delete object;
		return mesh;
	}
	else if (fullpath.Find(".3ds") != -1)
	{
		return LoadMesh3DS(file);
	}
	else {
		MeshLoaderB3D meshLoader;
		meshLoader.Load(fullpath);
		return meshLoader._mesh;
	}

	return NULL;
}

Texture* ResourceSystem::AddText( const char* text )
{
	if( Sys_DrawText(text, &textContent))
	{
		// get the texture pixels, width, height
		Texture* texture = new Texture;
		texture->Init(textContent.w, textContent.h, textContent.pData);	
		return texture;
	}
	else
	{
		Sys_Printf("sys_drawtext error %s\n", text);
		return FindTexture(DEFAULT_TEXTURE_PATH);
	}
}

Material* ResourceSystem::FindMaterial( const char* file )
{
	Material* mtr;
	auto it = _materials.Get(file);
	if( it != NULL ) {
		mtr = (Material*) it;
		return mtr;
	}

	lfStr fullPath = _searchDir + file;
	mtr = new Material();
	mtr->SetName(file);
	const char* buffer = F_ReadFileData(fullPath);

	if (buffer == NULL)
	{
		Sys_Error("add material failed %s, file data is null\n", file);
		return NULL;
	}

	mtr->LoadMemory(buffer);
	_materials.Put(file, mtr);
	return mtr;
}

static Image* LoadImage(lfStr filename, Image* image) {
	for (int i = 0; i < TexPluginCount; ++i)
	{
		if (filename.Find(loaderPlugin[i].name) != -1)
		{
			if( loaderPlugin[i].pFunc(filename.c_str(), image) )
				return image;
		}
	}
	return NULL;
}

Texture* ResourceSystem::AddCubeTexture(const char* px, const char* nx, const char* py, 
const char* ny, const char* pz, const char* nz)
{
	Texture* texture = NULL;

	Image images[6];
	LoadImage(px, &images[0]);
	LoadImage(nx, &images[1]);
	LoadImage(py, &images[2]);
	LoadImage(ny, &images[3]);
	LoadImage(pz, &images[4]);
	LoadImage(nz, &images[5]);

	texture = new Texture();
//	texture->InitCubeTexture(&images);

	return texture;
};

void ResourceSystem::SetSearchDir( const char* dir )
{
	_searchDir = dir;
}


