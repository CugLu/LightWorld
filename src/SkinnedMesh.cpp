#include "SkinnedMesh.h"
#include "Joint.h"
#include "Mesh.h"

SkinnedMesh::SkinnedMesh(Mesh* mesh, Joint* root, float totalFrame)
						:_mesh(mesh)
						,_rootJoint(root)
						,_totalFrame(totalFrame)
						,_currentFrame(0)

{
	init();
}

SkinnedMesh::~SkinnedMesh()
{
}

void SkinnedMesh::traverse(Joint* joint)
{
	if (joint == NULL)
		return;

	_allJoint.push_back(joint);
	for (unsigned int i = 0; i < joint->children.size(); ++i)
	{
		traverse(joint->children[i]);
	}

	_skinnedMeshPositions = new vec3[_mesh->_positions.size()];
}

void SkinnedMesh::init()
{
	traverse(_rootJoint);

	for (unsigned int i = 0; i < _allJoint.size(); i++)
	{
		Joint* joint = _allJoint.at(i);
		mat4 positionMatrix;
		positionMatrix.buildTranslate(joint->position);
		mat4 rotationMatrix;
		rotationMatrix = joint->rotation.toMatrix();
		mat4 LocalAniMatrix = positionMatrix*rotationMatrix;

		if (joint->parent)
		{
			joint->globalAnimatedMatrix = joint->parent->globalAnimatedMatrix*LocalAniMatrix;
		}
		else
		{
			joint->globalAnimatedMatrix = LocalAniMatrix;
		}

		joint->globalInvMatrix = joint->globalAnimatedMatrix.inverse();
	}

}

void SkinnedMesh::init(Mesh * mesh, Joint * root, float totalFrame)
{
	_mesh = mesh;
	_rootJoint = root;
	_totalFrame = totalFrame;
	_currentFrame = 0;
	init();
}

void SkinnedMesh::updateVertex(Joint* joint, float frame)
{
	if (joint)
	{
		vec3 position;
		quat rotation;
		joint->getFrame(frame, position, rotation);

		mat4 mat;
		if (joint->parent)
		{
			mat = joint->parent->globalAnimatedMatrix;
			joint->position = position;
		}

		mat4 positionMatrix;
		positionMatrix.buildTranslate(position);
		mat4 rotationMatrix;
		rotationMatrix = rotation.toMatrix();
		mat4 localAniMatrix = positionMatrix * rotationMatrix;

		if (joint->parent)
			joint->globalAnimatedMatrix = joint->parent->globalAnimatedMatrix * localAniMatrix;
		else
			joint->globalAnimatedMatrix = localAniMatrix;

		mat = joint->globalAnimatedMatrix * joint->globalInvMatrix;
		
		for (unsigned int i = 0; i < joint->vertexIndices.size(); ++i)
		{
			int vertex = joint->vertexIndices[i];
			vec3 p = _mesh->_positions[vertex];
			mat.transformVec3(p.x, p.y, p.z);
			_skinnedMeshPositions[vertex] = p * joint->vertexWeights[i];
		}
	}
}

void SkinnedMesh::updateMesh()
{
	for (unsigned int i = 0; i < _allJoint.size(); i++)
	{
		updateVertex(_allJoint.at(i), _currentFrame);
	}
}

void SkinnedMesh::update(float fElapsedTime)
{
	if (_mesh == nullptr) return;
	_currentFrame += fElapsedTime;

	if (_currentFrame > _endFrame)
		_currentFrame = _startFrame;

	updateMesh();
}

vec3* SkinnedMesh::getSkinnedmeshPositions()
{
	return _skinnedMeshPositions;
}
