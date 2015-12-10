#include "r_public.h"
#include "DrawVert.h"
#include "sys/sys_public.h"
#include "File.h"

static const int SHADOWMAP_DEPTH_SIZE = 1024;

srfTriangles_t * R_AllocStaticTriSurf( void )
{
	srfTriangles_t *tris = new srfTriangles_t;
	memset( tris, 0, sizeof( srfTriangles_t ) );
	return tris;
}

void R_AllocStaticTriSurfVerts( srfTriangles_t *tri, int numVerts ) {
	tri->verts = new DrawVert[ numVerts ];
}

material_t* R_AllocMaterail()
{
	material_t* material = new material_t;
	memset(material, 0, sizeof(material_t));
	return material;
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glIndex_t) * tri->numIndexes, tri->indexes, GL_STATIC_DRAW);

	// Clear the VBO state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

drawSurf_t* R_AllocDrawSurf()
{
	drawSurf_t* drawSurf = new drawSurf_t;
	memset(drawSurf, 0, sizeof(drawSurf_t));
	drawSurf->matModel.makeIdentity();
	return drawSurf;
}

drawSurf_t* R_GenerateFloor(float w, float h)
{
	float halfw = w/2;
	float halfh = h/2;
	drawSurf_t* surf = R_GenerateQuadSurf();
	surf->geo->verts[0].xyz = vec3(-halfw, 0.f, -halfh);
	surf->geo->verts[1].xyz = vec3(-halfw, 0.f, halfh);
	surf->geo->verts[2].xyz = vec3(halfw, 0.f, -halfh);
	surf->geo->verts[3].xyz = vec3(halfw, 0.f, halfh);

	return surf;
}

void R_GeneratePlane(srfTriangles_t* geo, float w, float h)
{
	R_GenerateQuad(geo);
	float halfw = w/2;
	float halfh = h/2;
	geo->verts[0].xyz = vec3(-halfw, 0.f, -halfh);
	geo->verts[1].xyz = vec3(-halfw, 0.f, halfh);
	geo->verts[2].xyz = vec3(halfw, 0.f, -halfh);
	geo->verts[3].xyz = vec3(halfw, 0.f, halfh);
}

void R_GenerateQuad( srfTriangles_t* geo )
{
	geo->vbo[0] = 0;
	geo->vbo[1] = 0;

	geo->numVerts = 4;
	 R_AllocStaticTriSurfVerts(geo, 4);

	geo->verts[0].st = vec2(0.f, 1.f);
	geo->verts[1].st = vec2(0.f, 0.f);
	geo->verts[2].st = vec2(1.f, 1.f);
	geo->verts[3].st = vec2(1.f, 0.f);

	geo->numIndexes = 6;
	geo->indexes = new glIndex_t[6];
	geo->indexes[0] = 0;
	geo->indexes[1] = 1;
	geo->indexes[2] = 2;
	geo->indexes[3] = 2;
	geo->indexes[4] = 1;
	geo->indexes[5] = 3;
}

void R_GenerateBox( srfTriangles_t* geo, float sx, float sy, float sz)
{
	geo->vbo[0] = 0;
	geo->vbo[1] = 0;

	geo->numVerts = 8;
	R_AllocStaticTriSurfVerts(geo, 8);

	geo->verts[0].xyz = vec3(0, sy, sz);
	geo->verts[1].xyz = vec3(0, 0, sz);
	geo->verts[2].xyz = vec3(sx, sy, sz);
	geo->verts[3].xyz = vec3(sx, 0, sz);
	geo->verts[4].xyz = vec3(0, sy, 0);
	geo->verts[5].xyz = vec3(0, 0, 0);
	geo->verts[6].xyz = vec3(sx, sy, 0);
	geo->verts[7].xyz = vec3(sx, 0, 0);

	unsigned short indices[] = {   0, 1, 2, 2, 1, 3,
								5, 4, 7, 7, 4, 6,

								2, 3, 6, 6, 3, 7,
								//5, 1, 4, 4, 1, 0,

								//4, 0, 6, 6, 0, 2,
								//1, 5, 3, 3, 5, 7
						};

	//unsigned short indices[] = {   0, 1, 2, 2, 1, 3};
	geo->numIndexes = sizeof(indices) / sizeof(unsigned short);

	geo->indexes = new glIndex_t[geo->numIndexes];
	memcpy(geo->indexes, indices, geo->numIndexes);
}

shadowMap_t* R_GenerateShadowMap()
{
	shadowMap_t* shadowMap = new shadowMap_t;

	// Create texid
	glGenTextures(1, &shadowMap->texId);
	glBindTexture(GL_TEXTURE_2D, shadowMap->texId);
	glTexStorage2D( GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, SHADOWMAP_DEPTH_SIZE, SHADOWMAP_DEPTH_SIZE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glGenFramebuffers(1, &shadowMap->fbo);    
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMap->fbo);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap->texId, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return shadowMap;
}

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
	positionMatrix.buildTranslate(joint->position);
	mat4 rotationMatrix;
	rotationMatrix = joint->rotation.toMatrix();
	mat4 localAniMatrix = positionMatrix*rotationMatrix;

	if (joint->parent)
		joint->globalAnimatedMatrix = joint->parent->globalAnimatedMatrix*localAniMatrix;
	else
		joint->globalAnimatedMatrix = localAniMatrix;

	joint->globalInvMatrix = joint->globalAnimatedMatrix.inverse();
	
	for (unsigned int i = 0; i < joint->vertexIndices.size(); ++i) {
		int vertex = joint->vertexIndices[i];
		vec3 p = geo->verts[vertex].xyz / joint->vertexWeights[i];
		joint->globalInvMatrix.transformVec3(p.x, p.y, p.z);
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

	geo->basePoses = new vec3[geo->numVerts];

	R_InitPoses(geo, joint);
}

void R_UpdateGeoPoses(srfTriangles_t* geo, Joint* joint, float frame)
{
	if (geo == NULL || joint == NULL)
		return;

	vec3 position;
	quat rotation;
	joint->GetFrame(frame, position, rotation);

	//lfFile file;
	//file.Open("d:/2.txt", "a");
	//file.WriteString("%s %f %f %f\n", joint->name.c_str(), position.x, position.y, position.z);

	mat4 positionMatrix;
	positionMatrix.buildTranslate(position);
	mat4 rotationMatrix;
	rotationMatrix = rotation.toMatrix();
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
		vec3 p = geo->basePoses[vertex];
		mat.transformVec3(p.x, p.y, p.z);
		geo->verts[vertex].xyz = p * joint->vertexWeights[i];
		//file.WriteString("%f %f %f\n", geo->verts[vertex].xyz.x, geo->verts[vertex].xyz.y, geo->verts[vertex].xyz.z);
	}
}

vec2 R_WorldToScreenPos( vec3 pos, mat4* viewProj, int screenwidth, int screenheight )
{
	vec4 out = (*viewProj) * vec4(pos, 1.0f);

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

	vec2 screenPos;
	screenPos.x = out.x;
	screenPos.y = out.y;

	return screenPos;
}

