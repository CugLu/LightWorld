#include "r_public.h"
#include "DrawVert.h"
#include "sys/sys_public.h"
#include "common/File.h"
#include "common/Plane.h"
#include "common/mem.h"
#include "common/Collision.h"

static const int SHADOWMAP_DEPTH_SIZE = 1024;

srfTriangles_t * R_AllocStaticTriSurf( void )
{
	srfTriangles_t *tris = new srfTriangles_t;
	memset( tris, 0, sizeof( srfTriangles_t ) );
	return tris;
}

void R_AllocStaticTriSurfVerts( srfTriangles_t *tri, int numVerts ) {
	if(tri->verts)
		delete[] tri->verts;

	tri->verts = new DrawVert[ numVerts ];
}

void R_AllocStaticTriSurfPlanes( srfTriangles_t* tri, int num )
{
	tri->facePlanes = (Plane*)mem_alloc(sizeof(Plane) * num);
}

void R_GenerateGeometryVbo( srfTriangles_t *tri )
{
	if (tri->vbo[0] != 0)
		glDeleteBuffers(1, &tri->vbo[0]);

	if (tri->vbo[1] != 0)
		glDeleteBuffers(1, &tri->vbo[0]);

	glGenBuffers(1, &tri->vbo[0]);
	glGenBuffers(1, &tri->vbo[1]);

	// Stick the data for the vertices into its VBO
	glBindBuffer(GL_ARRAY_BUFFER, tri->vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(DrawVert) * tri->numVerts, tri->verts, GL_STATIC_DRAW);

	// Stick the data for the indices into its VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri->vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glIndex_t) * tri->numIndices, tri->indices, GL_STATIC_DRAW);

	// Clear the VBO state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	R_DrawAllTris(tri);
}

drawSurf_t* R_AllocDrawSurf()
{
	drawSurf_t* drawSurf = new drawSurf_t;
	memset(drawSurf, 0, sizeof(drawSurf_t));
	drawSurf->matModel.MakeIdentity();
	return drawSurf;
}

drawSurf_t* R_GenerateFloor(float w, float h)
{
	float halfw = w/2;
	float halfh = h/2;
	drawSurf_t* surf = R_GenerateQuadSurf();
	surf->geo->verts[0].xyz = Vec3(-halfw, 0.f, -halfh);
	surf->geo->verts[1].xyz = Vec3(-halfw, 0.f, halfh);
	surf->geo->verts[2].xyz = Vec3(halfw, 0.f, -halfh);
	surf->geo->verts[3].xyz = Vec3(halfw, 0.f, halfh);

	return surf;
}

void R_GeneratePlane(srfTriangles_t* geo, float w, float h)
{
	R_GenerateQuad(geo);
	float halfw = w/2;
	float halfh = h/2;
	geo->verts[0].xyz = Vec3(-halfw, 0.f, -halfh);
	geo->verts[1].xyz = Vec3(-halfw, 0.f, halfh);
	geo->verts[2].xyz = Vec3(halfw, 0.f, -halfh);
	geo->verts[3].xyz = Vec3(halfw, 0.f, halfh);
}

void R_SetTextureUV(srfTriangles_t* geo, float u, float v)
{
	geo->verts[0].st = Vec2(0.f, v);
	geo->verts[1].st = Vec2(0.f, 0.f);
	geo->verts[2].st = Vec2(u, v);
	geo->verts[3].st = Vec2(u, 0.f);
}

void R_GenerateQuad( srfTriangles_t* geo )
{
	geo->vbo[0] = 0;
	geo->vbo[1] = 0;

	geo->numVerts = 4;
	R_AllocStaticTriSurfVerts(geo, 4);
	
	R_SetTextureUV(geo, 1.f, 1.f);

	geo->numIndices = 6;
	geo->indices = new glIndex_t[6];
	geo->indices[0] = 0;
	geo->indices[1] = 1;
	geo->indices[2] = 2;
	geo->indices[3] = 2;
	geo->indices[4] = 1;
	geo->indices[5] = 3;
}

void R_GenerateBox( srfTriangles_t* geo, float sx, float sy, float sz)
{
	geo->vbo[0] = 0;
	geo->vbo[1] = 0;

	geo->numVerts = 8;
	R_AllocStaticTriSurfVerts(geo, geo->numVerts );

	geo->verts[0].xyz = Vec3(0, sy, sz);
	geo->verts[1].xyz = Vec3(0, 0, sz);
	geo->verts[2].xyz = Vec3(sx, sy, sz);
	geo->verts[3].xyz = Vec3(sx, 0, sz);
	geo->verts[4].xyz = Vec3(0, sy, 0);
	geo->verts[5].xyz = Vec3(0, 0, 0);
	geo->verts[6].xyz = Vec3(sx, sy, 0);
	geo->verts[7].xyz = Vec3(sx, 0, 0);

	unsigned short indices[] = {   0, 1, 2, 2, 1, 3,
								5, 4, 7, 7, 4, 6,

								2, 3, 6, 6, 3, 7,
								5, 1, 4, 4, 1, 0,

								4, 0, 6, 6, 0, 2,
								1, 5, 3, 3, 5, 7
						};

	geo->numIndices = sizeof(indices) / sizeof(unsigned short);

	geo->indices = new glIndex_t[geo->numIndices];
	for (int i=0; i<geo->numIndices; i++)
	{
		geo->indices[i] = indices[i];
	}
}

//shadowMap_t* R_GenerateShadowMap()
//{
//	shadowMap_t* shadowMap = new shadowMap_t;
//
//	// Create texid
//	glGenTextures(1, &shadowMap->texId);
//	glBindTexture(GL_TEXTURE_2D, shadowMap->texId);
//	glTexStorage2D( GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, SHADOWMAP_DEPTH_SIZE, SHADOWMAP_DEPTH_SIZE);
//
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//
//	glGenFramebuffers(1, &shadowMap->fbo);    
//	glBindFramebuffer(GL_FRAMEBUFFER, shadowMap->fbo);
//	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap->texId, 0);
//	glDrawBuffer(GL_NONE);
//	glReadBuffer(GL_NONE);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//	return shadowMap;
//}

mat4 R_BillboardModelView( mat4& model, mat4& view )
{
	mat4 mat;
	mat = view * model;

	for(int i=0; i<3; i++ ) 
		for(int j=0; j<3; j++ ) 
		{
			if ( i==j )
				mat.m[i*4+j] = 1.0;
			else
				mat.m[i*4+j] = 0.0;
		}
	
	return mat;
}

void R_BoundTriSurf( srfTriangles_t* tri )
{
	for (int i=0; i<tri->numVerts; i++)
	{
		tri->aabb.AddPoint(tri->verts[i].xyz);
	}
}

drawSurf_t* R_GenerateQuadSurf()
{
	drawSurf_t* surf = R_AllocDrawSurf();
	surf->geo = R_AllocStaticTriSurf();

	R_GenerateQuad(surf->geo);
	return surf;
}


static void R_InitPoses(srfTriangles_t* geo, Joint* joint)
{
	mat4 positionMatrix;
	positionMatrix.BuildTranslate(joint->position);
	mat4 rotationMatrix;
	rotationMatrix = joint->rotation.ToMatrix();
	mat4 localAniMatrix = positionMatrix*rotationMatrix;

	if (joint->parent)
		joint->globalAnimatedMatrix = joint->parent->globalAnimatedMatrix*localAniMatrix;
	else
		joint->globalAnimatedMatrix = localAniMatrix;

	joint->globalInvMatrix = joint->globalAnimatedMatrix.Inverse();
	
	for (unsigned int i = 0; i < joint->vertexIndices.size(); ++i) {
		int vertex = joint->vertexIndices[i];
		Vec3 p = geo->verts[vertex].xyz / joint->vertexWeights[i];
		joint->globalInvMatrix.TransformVec3(p.x, p.y, p.z);
		geo->basePoses[vertex] = p;
		//geo->basePoses[vertex] = geo->verts[vertex].xyz;
	}

	for (unsigned int i = 0; i < joint->children.size(); ++i)
		R_InitPoses(geo, joint->children[i]);
}

void R_InitBasePoses(srfTriangles_t* geo, Joint* joint)
{

	if (geo->basePoses != NULL)
		delete[] geo->basePoses;

	geo->basePoses = new Vec3[geo->numVerts];

	R_InitPoses(geo, joint);
}

void R_UpdateGeoPoses(srfTriangles_t* geo, Joint* joint, float frame)
{
	if (geo == NULL || joint == NULL)
		return;

	Vec3 position;
	Quat rotation;
	joint->GetFrame(frame, position, rotation);

	//lfFile file;
	//file.Open("d:/2.txt", "a");
	//file.WriteString("%s %f %f %f\n", joint->name.c_str(), position.x, position.y, position.z);

	mat4 positionMatrix;
	positionMatrix.BuildTranslate(position);
	mat4 rotationMatrix;
	rotationMatrix = rotation.ToMatrix();
	mat4 localAniMatrix = positionMatrix * rotationMatrix;

	if (joint->parent)
		joint->globalAnimatedMatrix = joint->parent->globalAnimatedMatrix * localAniMatrix;
	else
		joint->globalAnimatedMatrix = localAniMatrix;

	mat4 mat = joint->globalAnimatedMatrix;

	//file.WriteString("%f %f %f\n", mat.m[12], mat.m[13], mat.m[14]);
	for (unsigned int i = 0; i < joint->vertexIndices.size(); ++i)
	{
		int vertex = joint->vertexIndices[i];
		Vec3 p = geo->basePoses[vertex];
		mat.TransformVec3(p.x, p.y, p.z);
		geo->verts[vertex].xyz = p * joint->vertexWeights[i];
		//file.WriteString("%f %f %f\n", geo->verts[vertex].xyz.x, geo->verts[vertex].xyz.y, geo->verts[vertex].xyz.z);
	}
}

Vec2 R_WorldToScreenPos( Vec3 pos, mat4* viewProj, int screenwidth, int screenheight )
{
	Vec4 out = (*viewProj) * Vec4(pos, 1.0f);

	out.x /= out.w;
	out.y /= out.w;
	out.z /= out.w;

	// Map x, y and z to range 0-1
	out.x = out.x * 0.5f + 0.5f;
	out.y = out.y * 0.5f + 0.5f;
	out.z = out.z * 0.5f + 0.5f;

	// Map x,y to viewport
	out.x = out.x * screenwidth;
	out.y = (1-out.y) * screenheight;

	Vec2 screenPos;
	screenPos.x = out.x;
	screenPos.y = out.y;

	return screenPos;
}

void R_DrawAllTris( srfTriangles_t *tri )
{
	tri->drawBegin = 0;
	tri->drawCount = tri->numIndices;
}

void R_PickTri( srfTriangles_t* tri, Vec3 orig, Vec3 dir)
{
	if (tri->bPickUp)
		return;

	tri->numPickIndices = 0;
	float t, u, v;
	for (int i=0; i<tri->numIndices; i+=3) {
		int	i1, i2, i3;
		i1 = tri->indices[i + 0];
		i2 = tri->indices[i + 1];
		i3 = tri->indices[i + 2];
		Vec3* v0 = &tri->verts[i1].xyz;
		Vec3* v1 = &tri->verts[i2].xyz;
		Vec3* v2 = &tri->verts[i3].xyz;

		if( IntersectTriangle(orig, dir, *v0, *v1, *v2, &t, &u, &v) ) {
			tri->bPickUp = true;
			tri->pickIndices[tri->numPickIndices++] = i1;
			tri->pickIndices[tri->numPickIndices++] = i2;
			tri->pickIndices[tri->numPickIndices++] = i3;
		}
	}
}

void R_CreateSilIndice( srfTriangles_t* tri )
{
	int		c_removed, c_unique;
	int		*remap;
	int		i, j, hashKey;
	const DrawVert *v1, *v2;

	remap = (int*)malloc( tri->numVerts * sizeof( remap[0] ) );

	c_removed = 0;
	c_unique = 0;
	for ( i = 0 ; i < tri->numVerts ; i++ ) {
		v1 = &tri->verts[i];

		// see if there is an earlier vert that it can map to
		int j = 0;
		bool found = false;
		for ( j=0; j<i; ++j)
		{
			int k = remap[j];
			v2 = &tri->verts[k];
			if ( v2->xyz[0] == v1->xyz[0]
			&& v2->xyz[1] == v1->xyz[1]
			&& v2->xyz[2] == v1->xyz[2] ) {
				c_removed++;
				remap[i] = k;
				found = true;
				break;
			}
		}

		if ( !found ) {
			c_unique++;
			remap[i] = i;
		}
	}

	tri->silIndices = new glIndex_t[tri->numIndices];
	for ( i = 0; i < tri->numIndices; i++ ) {
		tri->silIndices[i] = remap[tri->indices[i]];
	}

	free(remap);
}
