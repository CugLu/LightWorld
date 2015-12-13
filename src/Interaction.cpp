#include "Interaction.h"

#define	MAX_SHADOW_INDEXES		0x18000
#define	MAX_SHADOW_VERTS		0x18000
static int	numShadowIndexes;
static unsigned short shadowIndices[MAX_SHADOW_INDEXES];

//array<unsigned short> contour;
bool isFrontFace(Vec3 v0, Vec3 v1, Vec3 v2, Vec3 light)
{
	Vec3 normal = (v1 - v0).cross(v2 - v0);
	float d = normal.dot(light);
	return d <= 0;
}
//
//bool isEqualEdge(u16 l0, u16 l1, u16 r0, u16 r1)
//{
//	vec3& v0 = gMesh->_positions[l0];
//	vec3& v1 = gMesh->_positions[l0];
//	vec3& vv0 = gMesh->_positions[r0];
//	vec3& vv1 = gMesh->_positions[r1];
//
//	return ((v0 == vv0 && v1 == vv1))
//		|| ((v1 == vv0 && v0 == vv1));
//}
//
//void createEdgesAndCaps(vec3 light, vec3* vertices, unsigned short* indices, int faceCount)
//{
//	// silhouette dectection
//	contour.clear();
//	gShadowVolume.clear();
//	for (int i = 0; i < faceCount; ++i)
//	{
//		const unsigned short i0 = indices[i * 3 + 0];
//		const unsigned short i1 = indices[i * 3 + 1];
//		const unsigned short i2 = indices[i * 3 + 2];
//
//		const vec3 v0 = vertices[i0];
//		const vec3 v1 = vertices[i1];
//		const vec3 v2 = vertices[i2];
//
//		vec3 lightDir(-light.x, -light.y, -light.z);
//		bool bFrontFace = isFrontFace(v0, v1, v2, lightDir);
//		if (bFrontFace)
//		{
//			gShadowVolume.push_back(v0);
//			gShadowVolume.push_back(v1);
//			gShadowVolume.push_back(v2);
//			for (int e = 0; e < 3; ++e)
//			{
//				const u16 ii0 = indices[i * 3 + e];
//				const u16 ii1 = indices[i * 3 + (e + 1) % 3];
//
//				unsigned int j = 0;
//				for (j = 0; j < contour.size(); j += 2)
//				{
//					if (isEqualEdge(contour[j], contour[j + 1], ii0, ii1))
//					{
//						contour[j] = contour[j + 1] = 0;
//						break;
//					}
//				}
//
//				if (j >= contour.size())
//				{
//					contour.push_back(ii0);
//					contour.push_back(ii1);
//				}
//			}
//		}
//
//	}
//
//	// filter the removed edge;
//	array<u16> newContour;
//	for (unsigned int i = 0; i < contour.size(); i+=2)
//	{
//		//printf("%d, %d\n", contour[i], contour[i + 1]);
//		if (contour[i] != 0 || contour[i + 1] != 0)
//		{
//			newContour.push_back(contour[i]);
//			newContour.push_back(contour[i + 1]);
//			//			printf("[%d %d], ", contour[i], contour[i + 1]);
//		}
//	}
//	contour = newContour;
//
//	// adjacency
//	// create side face
//	for (unsigned int i = 0; i < contour.size(); i+=2)
//	{
//		vec3 v1 = vertices[indices[contour[i]]];
//		vec3 v2 = vertices[indices[contour[i+1]]];
//		vec3 v3 = v1 + (v1 - light).normalize()*Infinity;
//		vec3 v4 = v2 + (v2 - light).normalize()*Infinity;
//
//		gShadowVolume.push_back(v3);
//		gShadowVolume.push_back(v2);
//		gShadowVolume.push_back(v1);
//
//		gShadowVolume.push_back(v3);
//		gShadowVolume.push_back(v4);
//		gShadowVolume.push_back(v2);
//	}
//};

srfTriangles_t* R_GenerateCaps( Vec3 light, srfTriangles_t* tri )
{
	numShadowIndexes = 0;
	int numPlanes = tri->numIndexes / 3; 
	for (int i=0; i<numPlanes; ++i)
	{
		unsigned short i0 = tri->indexes[i];
		unsigned short i1 = tri->indexes[i+1];
		unsigned short i2 = tri->indexes[i+2];

		if (isFrontFace(tri->verts[i0].xyz, tri->verts[i1].xyz, tri->verts[i2].xyz, light))
		{
			shadowIndices[numShadowIndexes++] = i0;
			shadowIndices[numShadowIndexes++] = i1;
			shadowIndices[numShadowIndexes++] = i2;
		}
	}
	return NULL;
}
