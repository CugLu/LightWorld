#include "Model.h"
#include "Mesh.h"
#include "ResourceSystem.h"
#include "r_public.h"

Model::Model()
{
	Init();
}

Model::~Model()
{

}

void Model::Init()
{
	_drawSurf = R_AllocDrawSurf();
	_drawSurf->shaderParms = R_AllocMaterail();
}

void Model::SetPosition(float x, float y, float z)
{
	_position.set(x, y, z);
	_drawSurf->matModel.buildTranslate(x, y, z);
}

Vec3 Model::GetPosition()
{
	return _position;
}

void Model::SetFile( const char* filename )
{
	Mesh* mesh = _resourceSys->AddMesh(filename);
	mesh->GenerateNormals();
	mesh->CalcBounds();
	_drawSurf->geo = mesh->GetGeometries(0);

	_drawSurf->shaderParms->tex = _resourceSys->AddTexture("0.png");
	R_GenerateGeometryVbo(_drawSurf->geo);

	_drawSurf->mtr = _resourceSys->AddMaterial("../media/mtr/position.mtr");
}

void Model::SetViewProj( mat4* viewProj )
{
	_drawSurf->viewProj = viewProj;
}



#include "common/Joint.h"
AniModel::AniModel():_root(NULL), _startFrame(0), _currentFrame(0), _isLoop(false)
{
	Init();
}

AniModel::~AniModel()
{
	
}

void AniModel::Init()
{
	_drawSurf = R_AllocDrawSurf();
	_drawSurf->shaderParms = R_AllocMaterail();
}

void AniModel::SetFile( const char* filename )
{
	Mesh* mesh = _resourceSys->AddMesh(filename);
	mesh->GenerateNormals();
	mesh->CalcBounds();
	//AddStaticModel(model);
	_drawSurf->geo = mesh->GetGeometries(0);

	_drawSurf->shaderParms->tex = _resourceSys->AddTexture("0.png");
	R_GenerateGeometryVbo(_drawSurf->geo);

	_root = mesh->GetRootJoint();

	_endFrame = mesh->GetNumFrames();

	R_InitBasePoses(_drawSurf->geo, _root);
}


void AniModel::Update( float elsepedtime )
{
	if (_root == NULL) 
		return;
	_currentFrame += elsepedtime;

	if (_currentFrame > _endFrame)
	{
		if (_isLoop)
			_currentFrame = _startFrame;
		else
			return;
	}

	UpdateJointPoses(_root);

	R_GenerateGeometryVbo(_drawSurf->geo);
}

void AniModel::UpdateJointPoses( Joint* joint )
{
	if (joint == NULL)
		return;

	R_UpdateGeoPoses(_drawSurf->geo, joint, _currentFrame);

	for (unsigned int i = 0; i < joint->children.size(); ++i)
		UpdateJointPoses(joint->children[i]);
}

void AniModel::SetPosition(float x, float y, float z)
{
	_position.set(x, y, z);
	_drawSurf->matModel.buildTranslate(x, y, z);
}

Vec3 AniModel::GetPosition()
{
	return _position;
}

void AniModel::SetViewProj( mat4* viewProj )
{
	_drawSurf->viewProj = viewProj;
}

void AniModel::Play(const char* name, bool isLoop)
{
	AniAction* animation = (AniAction*)_animations.Get(name);
	_currentFrame = animation->startframe;
	_startFrame = animation->startframe;
	_endFrame = animation->endframe;
	_isLoop = isLoop;
}

void AniModel::PushAnimation( const char* name, int start, int end )
{
	AniAction *animation = new AniAction;
	animation->name = name;
	animation->startframe = start;
	animation->endframe = end;

	_animations.Put(name, animation);
}
