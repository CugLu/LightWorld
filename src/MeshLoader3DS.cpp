#include "MeshLoader3DS.h"
#include "sys/sys_public.h"
#include "common/File.h"
#include "Mesh.h"
#include "common/String.h"
#include "DrawVert.h"


//MAIN CHUNK 0x4D4D
//	3D EDITOR CHUNK 0x3D3D
//	OBJECT BLOCK 0x4000
//	TRIANGULAR MESH 0x4100
//	VERTICES LIST 0x4110
//	FACES DESCRIPTION 0x4120
//	FACES MATERIAL 0x4130
//	MAPPING COORDINATES LIST 0x4140
//	SMOOTHING GROUP LIST 0x4150
//	LOCAL COORDINATES SYSTEM 0x4160
//	LIGHT 0x4600
//	SPOTLIGHT 0x4610
//	CAMERA 0x4700
//	MATERIAL BLOCK 0xAFFF
//	MATERIAL NAME 0xA000
//	AMBIENT COLOR 0xA010
//	DIFFUSE COLOR 0xA020
//	SPECULAR COLOR 0xA030
//	TEXTURE MAP 1 0xA200
//	BUMP MAP 0xA230
//	REFLECTION MAP 0xA220
//	[SUB CHUNKS FOR EACH MAP]
//MAPPING FILENAME 0xA300
//	MAPPING PARAMETERS 0xA351
//	KEYFRAMER CHUNK 0xB000
//	MESH INFORMATION BLOCK 0xB002
//	SPOT LIGHT INFORMATION BLOCK 0xB007
//	FRAMES (START AND END) 0xB008
//	OBJECT NAME 0xB010
//	OBJECT PIVOT POINT 0xB013
//	POSITION TRACK 0xB020
//	ROTATION TRACK 0xB021
//	SCALE TRACK 0xB022
//	HIERARCHY POSITION 0xB030
//
#define MAINCHUNK_3DS 0x4D4D

#define MASHDATA_3DS 0x3D3D
#define VERSION_3DS 0x0002

#define OBJNAME_3DS 0x4000
#define MESH_DATA_3DS 0x4100
#define VERTEX_DATA_3DS 0x4110
#define INDEX_DATA_3DS 0x4120
#define TEXMAP_DATA_3DS 0x4140

Mesh* LoadMesh3DS(const char* filename)
{
	lfFile* file = new lfFile;
	if ( !file->Open(filename) )
		return NULL;

	Mesh* mesh = new Mesh;
	// create drawsurf
	srfTriangles_s* geo = mesh->AllocGeo();

	while (file->Tell() < file->Length())
	{
		unsigned short chunkId = file->ReadUnsignedShort();
		unsigned int chunkLen = file->ReadUnsignedInt();

		switch (chunkId)
		{
		case MAINCHUNK_3DS:
			break;
		case MASHDATA_3DS:
			break;

		case OBJNAME_3DS:
			{
				lfStr str = file->ReadString();
				Sys_Printf("%s\n", str.c_str());
			}
			break;

		case MESH_DATA_3DS:
			break;

		case VERTEX_DATA_3DS:
			{
				unsigned short numVerts = file->ReadUnsignedShort();
				if (geo->numVerts == 0)
				{
					geo->numVerts = numVerts;
					R_AllocStaticTriSurfVerts(geo, geo->numVerts);
				}
				for(int i=0; i<geo->numVerts; ++i)
				{
					geo->verts[i].xyz = file->ReadVec3();
					//Sys_Printf("%f %f %f ", geo->verts[i].xyz.x, geo->verts[i].xyz.y, geo->verts[i].xyz.z);
				}

				Sys_Printf("numVert : %d\n", geo->numVerts);
			}
			break;

		case INDEX_DATA_3DS:
			{
				unsigned short numTri = file->ReadUnsignedShort();
				geo->numIndexes = numTri * 3;
				geo->indexes = new glIndex_t[geo->numIndexes];
				for(int i=0; i<numTri; ++i)
				{
					geo->indexes[i*3] = file->ReadUnsignedShort();
					geo->indexes[i*3+1] = file->ReadUnsignedShort();
					geo->indexes[i*3+2] = file->ReadUnsignedShort();
					unsigned short flag = file->ReadUnsignedShort();
					//Sys_Printf("%d %d %d ", geo->indexes[i], geo->indexes[i+1], geo->indexes[i+2]);
				}

				Sys_Printf("numTri : %d\n", numTri);
			}
			break;

		case TEXMAP_DATA_3DS:
			{
				unsigned short numUV = file->ReadUnsignedShort();
				if (geo->numVerts == 0)
				{
					geo->numVerts = numUV;
					R_AllocStaticTriSurfVerts(geo, geo->numVerts);
				}

				Sys_Printf("numUV : %d\n", numUV);

				for(int i=0; i<numUV; ++i)
				{
					float u = file->ReadFloat();
					float v = file->ReadFloat();
					geo->verts[i].st = Vec2(u, v);
				}
			}
			break;
		default:
			{
				file->Seek(chunkLen-6, FS_SEEK_CUR);
			}
		}
	}

	delete file;
	return mesh;
}