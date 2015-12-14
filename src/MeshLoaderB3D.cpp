#include "MeshLoaderB3D.h"
#include "common/Joint.h"
#include "common/File.h"
#include "sys/sys_public.h"
#include "Mesh.h"
#include "DrawVert.h"

MeshLoaderB3D::MeshLoaderB3D() {
}

MeshLoaderB3D::~MeshLoaderB3D() {
}

bool MeshLoaderB3D::Load(const char* file) {
	_file = new lfFile;
	if( !_file->Open(file) )
		return false;

	_mesh = new Mesh;
	lfStr head = ReadChunk();
	int nB3DVersion = _file->ReadInt();

	Sys_Printf("load b3d file %s, version %s %d\n", file, head.c_str(), nB3DVersion);

	while( CheckSize() ) {
		lfStr t = ReadChunk();
		if (t == "TEXS")
			ReadTexs();
		else if (t == "BRUS")
			ReadBrus();
		else if (t == "NODE")
			_mesh->SetJoint(ReadNode());

		ExitChunk();
	}

	delete _file;
	_file = NULL;

	// The MESH chunk describes a mesh. 
	// A mesh only has one VRTS chunk, but potentially many TRIS chunks.
	srfTriangles_t* tri = _mesh->GetGeometries(0);
	tri->numIndexes = _indices.size();
	_indices.set_free_when_destroyed(false);
	tri->indexes = _indices.pointer();

	return true;
}


bool MeshLoaderB3D::ReadVrts() {
	const int max_tex_coords = 3;
	int flags, tex_coord_sets, tex_coord_set_size;

	flags = _file->ReadInt();
	tex_coord_sets = _file->ReadInt();
	tex_coord_set_size = _file->ReadInt();

	if (tex_coord_sets >= max_tex_coords || tex_coord_set_size >= 4) // Something is wrong 
	{
		Sys_Printf("tex_coord_sets or tex_coord_set_size too big");
		return false;
	}

	//------ Allocate Memory, for speed -----------//

	int sizeOfVertex = 3;
	bool hasNormal = false;
	bool hasVertexColors = false;
	if (flags & 1) {
		hasNormal = true;
		sizeOfVertex += 3;
	}
	if (flags & 2) {
		sizeOfVertex += 4;
		hasVertexColors=true;
	}

	sizeOfVertex += tex_coord_sets*tex_coord_set_size;
	unsigned int size = _stack[_stack.size() - 1] - _file->Tell();
	//106407 16800
	unsigned int numVertex = size / sizeof(float) ;
	numVertex /= sizeOfVertex;

	srfTriangles_t* tri = _mesh->AllocGeo();
	tri->numVerts = numVertex;
	R_AllocStaticTriSurfVerts(tri, numVertex);

	int idx = 0;
	while( CheckSize()) {
		float color[4]={1.0f, 1.0f, 1.0f, 1.0f};
		tri->verts[idx].xyz = _file->ReadVec3();

		if (flags & 1) {
			tri->verts[idx].normal = _file->ReadVec3();
		}
		if (flags & 2) {
			color[0] = _file->ReadFloat();
			color[1] = _file->ReadFloat();
			color[2] = _file->ReadFloat();
			color[3] = _file->ReadFloat();
		}
		float u, v;
		for (int i = 0; i < tex_coord_sets; ++i) {
			//for (int j = 0; j < tex_coord_set_size; ++j)
			{
				u = _file->ReadFloat();
				v = 1.0f - _file->ReadFloat();
			}
		}

		tri->verts[idx].st = Vec2(u, v);
		idx++;
	}

	return true;
}

lfStr MeshLoaderB3D::ReadChunk()
{
	lfStr s;
	for (int i = 0; i < 4;++i) {
		s += char(_file->ReadChar());
	}
	
	unsigned int size = _file->ReadInt();
	unsigned int pos = _file->Tell();
	_stack.push_back(pos + size);
	return s;
}

bool MeshLoaderB3D::CheckSize()
{
	unsigned int pos = _file->Tell();
	unsigned int size = _stack[_stack.size() - 1];
	return size > pos;
}

void MeshLoaderB3D::ExitChunk()
{
	_curpos = _stack.end();
	_stack.erase(_stack.size() - 1);
}

void MeshLoaderB3D::ReadTexs()
{
	while (CheckSize()) {
		PrintTree("read texs \n");
		SB3dTexture tex;
		tex.TextureName = _file->ReadString();
		tex.Flags = _file->ReadInt();
		tex.Blend = _file->ReadInt();
		tex.Xpos = _file->ReadFloat();
		tex.Ypos = _file->ReadFloat();
		tex.Xscale = _file->ReadFloat();
		tex.Yscale = _file->ReadFloat();
		tex.Angle = _file->ReadFloat();
		_textures.push_back(tex);
	}
}

Joint* MeshLoaderB3D::ReadNode()
{
	Joint* joint = new Joint;

	lfStr str = _file->ReadString();
	PrintTree(str.c_str());

	Vec3 t = _file->ReadVec3();
	Vec3 s = _file->ReadVec3();
	Quat r = _file->ReadQuat();

	joint->name = str;
	joint->position = t;
	joint->scale = s;
	joint->rotation = r;

	while( CheckSize() ){
		lfStr t = ReadChunk();
		if( t=="MESH" ){
			ReadMesh();
		}else if( t=="BONE" ){
			ReadBone(joint);
		}else if( t=="ANIM" ){
			ReadAnim();
		}else if( t=="KEYS" ){
			ReadKey(joint);
		}else if( t=="NODE" ){
			Joint* child = ReadNode();
			//Sys_Printf("parent %s children %s\n", joint->name.c_str(), child->name.c_str());
			joint->children.push_back(child);
			child->parent = joint;
		}
		ExitChunk();
	}
	return joint;

}

void MeshLoaderB3D::ReadBrus()
{
	int n_texs = _file->ReadInt();
	if( n_texs<0 || n_texs>8 ){
		printf( "Bad texture count" );
	}
	while( CheckSize() ){
		lfStr name = _file->ReadString();
		PrintTree(name.c_str());
		Vec3 color = _file->ReadVec3();
		float alpha = _file->ReadFloat();
		float shiny = _file->ReadFloat();
		/*int blend=**/_file->ReadInt();
		int fx = _file->ReadInt();

		//Textures
		for( int i=0;i<n_texs;++i ){
			int texid= _file->ReadInt();
		}
	}
}

void MeshLoaderB3D::ReadBone( Joint* joint )
{
	int i = 0;
	while( CheckSize() ){
		int vertex = _file->ReadInt();
		float weight = _file->ReadFloat();
		joint->vertexIndices.push_back(vertex);
		joint->vertexWeights.push_back(weight);
		i++;
	}
	PrintTree("vertex count: %d", i);
}

void MeshLoaderB3D::ReadMesh() {
	/*int matid=*/_file->ReadInt();

	//printTree("mesh");
	while( CheckSize() ){
		lfStr t = ReadChunk();
		if( t=="VRTS" ){
			ReadVrts();
		}else if( t=="TRIS" ){
			ReadTris();
		}
		ExitChunk();
	}
}

void MeshLoaderB3D::PrintTree(const char *psz, ...) {
	//char sBuf[128];
	//va_list ap;
	//va_start(ap, psz);
	//vsnprintf_s(sBuf, 128, 128, psz, ap);
	//va_end(ap);

	//for (int i = 0; i < _stack.size();i++)
	//	Sys_Printf("-");

	//Sys_Printf(sBuf);
	//Sys_Printf("\n");
}

void MeshLoaderB3D::ReadTris(){
	int matid = _file->ReadInt();
	if( matid==-1 ){
		matid=0;
	}
	int size = _stack.end() - _file->Tell();
	int n_tris=size/12;

	for( int i=0;i<n_tris;++i ){
		int i0 = _file->ReadUnsignedInt();
		int i1 = _file->ReadUnsignedInt();
		int i2 = _file->ReadUnsignedInt();

		_indices.push_back(i0);
		_indices.push_back(i1);
		_indices.push_back(i2);
	}
}



void MeshLoaderB3D::ReadAnim(){
	/*int flags=*/_file->ReadInt();
	int frames = _file->ReadInt();
	/*float fps = */_file->ReadFloat();
	_mesh->SetNumFrames(frames);
}

void MeshLoaderB3D::ReadKey(Joint* joint)
{
	int flags = _file->ReadInt();
	while( CheckSize() ){
		int frame = _file->ReadInt();
		if (flags & 1){
			PositionKey k;
			k.position = _file->ReadVec3();
			k.frame = frame;
			joint->positionKeys.push_back(k);
		}
		if( flags & 2 ){
			ScaleKey k;
			k.position = _file->ReadVec3();
			k.frame = frame;
			joint->scaleKeys.push_back(k);
		}
		if( flags & 4 ){
			Quat r = _file->ReadQuat();
			r.w = -r.w; // fix bug
			RotationKey k;
			k.rotation = r;
			k.frame = frame;
			joint->rotationKeys.push_back(k);
		}
	}
}