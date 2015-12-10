#ifndef __DRAW_COMMON_H__
#define __DRAW_COMMON_H__
#include "r_public.h"

typedef void (*DrawFunc)(srfTriangles_t* geo);

void R_RenderPTPass(drawSurf_t* drawSur, DrawFunc drawFunc);

void R_RenderShadowMap(drawSurf_t* drawSur, DrawFunc drawFunc);

void R_RenderPhongPass(drawSurf_t* drawSurf, DrawFunc drawFunc);

void R_DrawPositonTex( srfTriangles_t* tri );

void R_DrawPositon( srfTriangles_t* tri );

void R_DrawPositionTexNorm( srfTriangles_t* tri );

void RB_DrawBounds( aabb3d* aabb3d );

void R_RenderBumpPass( drawSurf_t* drawSurf, DrawFunc drawFunc );

void R_DrawPositonTangent( srfTriangles_t* tri);

// draw common version 2
void R_RenderCommon(drawSurf_t* drawSurf);

//void R_DrawCommon( srfTriangles_t* tri, unsigned short *attri, unsigned short numAttri );

void R_Draw3DCoordinate();
#endif


