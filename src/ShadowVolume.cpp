#include "ShadowVolume.h"
typedef unsigned int u32;
typedef int s32;
typedef unsigned short u16;

static const float LINFINITY = 9999;

static bool isFrontFace(Vec3 v0, Vec3 v1, Vec3 v2, Vec3 light)
{
	Vec3 normal = (v1 - v0).cross(v2 - v0);
	float d = normal.dot(light);
	return d <= 0;
}


ShadowVolume::ShadowVolume(Vec3* vertices, unsigned short* indices, unsigned int faceCount):
	mVertices(vertices), mIndices(indices), mFaceCount(faceCount)
{
	mIsFrontFace = new bool[mFaceCount];
	mAdjacency = new int[faceCount*3];
	calculateAdjacency();
}


ShadowVolume::~ShadowVolume()
{
	if (mIsFrontFace)
	{
		delete[] mIsFrontFace;
		mIsFrontFace = 0;
	}
}

void ShadowVolume::createVolumeShadow()
{
	Vec3 light(5, 15, -5);
	s32 indexCount = mFaceCount*3;
	Array<Vec3>* svp = &mShadowVolume;
	svp->reallocate(indexCount*5);

	// We use triangle lists
	mEdges.set_used(indexCount*2);
	u32 numEdges = 0;

	numEdges=createEdgesAndCaps(light, svp);

	// for all edges add the near->far quads
	for (u32 i=0; i<numEdges; ++i)
	//for (u32 i=0; i<1; ++i)
	{
		const Vec3 &v1 = mVertices[mEdges[2*i+0]];
		const Vec3 &v2 = mVertices[mEdges[2*i+1]];
		const Vec3 v3(v1+(v1 - light).normalize()*LINFINITY);
		const Vec3 v4(v2+(v2 - light).normalize()*LINFINITY);

		// Add a quad (two triangles) to the vertex list
		svp->push_back(v1);
		svp->push_back(v2);
		svp->push_back(v3);

		svp->push_back(v2);
		svp->push_back(v4);
		svp->push_back(v3);
	}
}

int ShadowVolume::createEdgesAndCaps(Vec3 light, Array<Vec3>* svp)
{
	u32 indexCount = mFaceCount*3;
	u32 numEdges=0;
	const u32 faceCount = indexCount / 3;

	// Check every face if it is front or back facing the light.
	for (u32 i=0; i<faceCount; ++i)
	{
		const Vec3 v0 = mVertices[mIndices[3*i+0]];
		const Vec3 v1 = mVertices[mIndices[3*i+1]];
		const Vec3 v2 = mVertices[mIndices[3*i+2]];

		mIsFrontFace[i]=isFrontFace(v2,v1,v0, light);

		if (mIsFrontFace[i])
		{
			// add front cap from light-facing faces
			svp->push_back(v2);
			svp->push_back(v1);
			svp->push_back(v0);

			// add back cap
			const Vec3 i0 = v0+(v0-light).normalize()*LINFINITY;
			const Vec3 i1 = v1+(v1-light).normalize()*LINFINITY;
			const Vec3 i2 = v2+(v2-light).normalize()*LINFINITY;

			svp->push_back(i0);
			svp->push_back(i1);
			svp->push_back(i2);
		}
	}

	// Create edges
	for (u32 i=0; i<faceCount; ++i)
	{
		// check all front facing faces
		if (mIsFrontFace[i] == true)
		{
			const u16 wFace0 = mIndices[3*i+0];
			const u16 wFace1 = mIndices[3*i+1];
			const u16 wFace2 = mIndices[3*i+2];

			const u16 adj0 = mAdjacency[3*i+0];
			const u16 adj1 = mAdjacency[3*i+1];
			const u16 adj2 = mAdjacency[3*i+2];

			// add edges if face is adjacent to back-facing face
			// or if no adjacent face was found
			if (adj0 == i || mIsFrontFace[adj0] == false)
			{
				// add edge v0-v1
				mEdges[2*numEdges+0] = wFace0;
				mEdges[2*numEdges+1] = wFace1;
				++numEdges;
			}

			if (adj1 == i || mIsFrontFace[adj1] == false)
			{
				// add edge v1-v2
				mEdges[2*numEdges+0] = wFace1;
				mEdges[2*numEdges+1] = wFace2;
				++numEdges;
			}

			if (adj2 == i || mIsFrontFace[adj2] == false)
			{
				// add edge v2-v0
				mEdges[2*numEdges+0] = wFace2;
				mEdges[2*numEdges+1] = wFace0;
				++numEdges;
			}
		}
	}
	return numEdges;
}

void ShadowVolume::createSideFace()
{

}

void ShadowVolume::calculateAdjacency()
{
	s32 indexCount = mFaceCount*3;
	// go through all faces and fetch their three neighbours
	for (u32 f=0; f<mFaceCount*3; f+=3)
	{
		for (u32 edge = 0; edge<3; ++edge)
		{
			const Vec3& v1 = mVertices[mIndices[f+edge]];
			const Vec3& v2 = mVertices[mIndices[f+((edge+1)%3)]];

			// now we search an_O_ther _F_ace with these two
			// vertices, which is not the current face.
			u32 of;

			for (of=0; of<mFaceCount*3; of+=3)
			{
				// only other faces
				if (of != f)
				{
					bool cnt1 = false;
					bool cnt2 = false;

					for (s32 e=0; e<3; ++e)
					{
						if (v1 == mVertices[mIndices[of+e]])
							cnt1=true;

						if (v2 == mVertices[mIndices[of+e]])
							cnt2=true;
					}
					// one match for each vertex, i.e. edge is the same
					if (cnt1 && cnt2)
						break;
				}
			}

			// no adjacent edges -> store face number, else store adjacent face
			if (of >= indexCount)
				mAdjacency[f + edge] = f/3;
			else
				mAdjacency[f + edge] = of/3;
		}
	}
}