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
static Texture* defaultTexture;
//--------------------------------------------------------------------------------------------
static Shader* LoadPostionShader()
{
	Shader* shader = new Shader;
	shader->LoadFromBuffer(position_vert, position_frag);
	shader->SetName("position");
	shader->BindAttribLocation(eAttrib_Position);
	shader->GetUniformLocation(eUniform_MVP);
	shader->GetUniformLocation(eUniform_Color);
	GL_CheckError("LoadPostionShader");
	return shader;
}

static Shader* LoadPositionTexShader()
{
	Shader* shader = new Shader;
	shader->LoadFromBuffer(positiontex_vert, positiontex_frag);
	shader->SetName("positionTex");
	shader->BindAttribLocation(eAttrib_Position);
	shader->BindAttribLocation(eAttrib_TexCoord);
	shader->GetUniformLocation(eUniform_MVP);
	shader->GetUniformLocation(eUniform_Samper0);
	GL_CheckError("LoadPositionTexShader");
	return shader;
}

static Shader* LoadPhongShader()
{
	Shader* shader = new Shader;
	shader->LoadFromFile("../media/shader/phong.vert", "../media/shader/phong.frag");
	shader->SetName("phong");
	shader->BindAttribLocation(eAttrib_Position);
	shader->BindAttribLocation(eAttrib_TexCoord);
	shader->BindAttribLocation(eAttrib_Normal);

	shader->GetUniformLocation(eUniform_MVP);
	shader->GetUniformLocation(eUniform_EyePos);
	shader->GetUniformLocation(eUniform_LightPos);
	shader->GetUniformLocation(eUniform_ModelView);
	shader->GetUniformLocation(eUniform_InvModelView);
	shader->GetUniformLocation(eUniform_Samper0);
	GL_CheckError("load phong shader");
	return shader;
}

static Shader* LoadBumpShader()
{
	Shader* shader = new Shader;
	shader->LoadFromFile("../media/shader/bump.vert", "../media/shader/bump.frag");
	shader->SetName("bump");
	shader->BindAttribLocation(eAttrib_Position);
	shader->BindAttribLocation(eAttrib_TexCoord);
	shader->BindAttribLocation(eAttrib_Normal);
	shader->BindAttribLocation(eAttrib_Tangent);
	shader->BindAttribLocation(eAttrib_Binormal);

	shader->GetUniformLocation(eUniform_MVP);
	shader->GetUniformLocation(eUniform_EyePos);
	shader->GetUniformLocation(eUniform_LightPos);
	shader->GetUniformLocation(eUniform_ModelView);
	shader->GetUniformLocation(eUniform_InvModelView);
	shader->GetUniformLocation(eUniform_Samper0);
	shader->GetUniformLocation(eUniform_BumpMap);
	return shader;
}

static Shader* LoadBlurShader()
{
	Shader* shader = new Shader;
	shader->LoadFromFile("../media/blur.vs", "../media/blur.fs");
	shader->SetName("blur");
	shader->BindAttribLocation(eAttrib_Position);
	shader->BindAttribLocation(eAttrib_TexCoord);
	shader->GetUniformLocation(eUniform_MVP);
	shader->GetUniformLocation(eUniform_Samper0);
	return shader;
}


static ShaderPlugin shaderplugin[] = {
	{ eShader_Position, LoadPostionShader },
	{ eShader_PositionTex, LoadPositionTexShader },
	//{ eShader_Phong, LoadPhongShader },
	//{ eShader_Blur, LoadBlurShader  },
	//{ eShader_Bump, LoadBumpShader },
};
static int ShaderPluginCount = sizeof(shaderplugin) / sizeof(ShaderPlugin);

//--------------------------------------------------------------------------------------------

static sysTextContent_t textContent;


//ResourceManager* ResourceManager::sm_pSharedInstance = nullptr;
ResourceSystem::ResourceSystem()
{
}

ResourceSystem::~ResourceSystem()
{
	
}


Texture* ResourceSystem::AddTexture(const char* file)
{
	Texture* texture = NULL;

	lfStr fullPath = file;
	void* it = _textures.Get(fullPath);
    if( it != NULL ) {
		texture = (Texture*)it;
		return texture;
    }

	Image image;

	std::string basename(file);
    std::transform(basename.begin(), basename.end(), basename.begin(), ::tolower);
    
	for (int i = 0; i < TexPluginCount; ++i)
	{
		if (basename.find(loaderPlugin[i].name) == std::string::npos)
			continue;

		if( !loaderPlugin[i].pFunc(fullPath.c_str(), &image) )
		{
			Sys_Printf( "load image %s failed\n", fullPath.c_str() );
			return defaultTexture;
		}
		else
		{
			texture = new Texture();
			texture->Init(&image);

			_textures.Put(fullPath, texture);
			return texture;
		}
	}

	Sys_Printf( "load image %s failed\n", fullPath.c_str() );
	return defaultTexture;
};

Mesh* ResourceSystem::AddMesh(const char* file)
{
	lfStr str = file;
	if (str.Find(".lwo") != -1) { 
		unsigned int failId;
		int failedPos;
		lwObject* object = lwGetObject(file, &failId, &failedPos);

		Mesh* mesh = new Mesh;
		mesh->ConvertLWOToModelSurfaces(object);
		delete object;
		return mesh;
	}
	else if (str.Find(".3ds") != -1)
	{
		return LoadMesh3DS(file);
	}
	else {
		MeshLoaderB3D meshLoader;
		meshLoader.Load(file);
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
		return defaultTexture;
	}
}

Shader* ResourceSystem::AddShaderFromFile( const char* vfile, const char* ffile )
{
	Shader* shader = new Shader;
	shader->LoadFromFile(vfile, ffile);
	return shader;
}

Material* ResourceSystem::AddMaterial( const char* file )
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
	const char* buffer = F_ReadFileData(fullPath); //"../media/Position.mtr");

	if (buffer == NULL)
	{
		Sys_Error("add material failed %s, file data is null\n", file);
		return NULL;
	}

	mtr->LoadMemory(buffer);
	_materials.Put(fullPath, mtr);
	return mtr;
}

bool ResourceSystem::LoadGLResource()
{
	memset(_shaders, 0, 32);
	for (int i =0; i<ShaderPluginCount; i++)
	{
		_shaders[shaderplugin[i].name] = shaderplugin[i].func();
	}

	defaultTexture = AddTexture("../Media/nskinbl.jpg");
	return true;
}

Shader* ResourceSystem::FindShader( int shaderId )
{
	if (shaderId >= MAX_SHADER_COUNT && shaderId < 0)
	{
		Sys_Error("find shader out of bounds");
		return NULL;
	}	

	return _shaders[shaderId];
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

Texture* ResourceSystem::AddCubeTexture(const char* px, const char* nx, const char* py, const char* ny, const char* pz, const char* nz)
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

