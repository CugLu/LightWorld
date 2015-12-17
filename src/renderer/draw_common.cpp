#include "draw_common.h"
#include "Shader.h"
#include "Texture.h"
#include "DrawVert.h"
#include "sys/sys_public.h"
#include "Material.h"

//#define offsetof(s,m)   (size_t)&reinterpret_cast<const volatile char&>((((s *)0)->m))

void R_RenderPTPass( drawSurf_t* drawSur, DrawFunc drawFunc ) {
	srfTriangles_t* tri = drawSur->geo;
	material_t* material = drawSur->shaderParms;
	mat4 t = (*drawSur->viewProj) * drawSur->matModel;
	Shader* shader = material->shader;

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glUseProgram( shader->GetProgarm() );
	glUniformMatrix4fv( shader->GetUniform(eUniform_MVP), 1, GL_FALSE, &t.m[0] );
	glUniform1i( shader->GetUniform(eUniform_Samper0), 0 );
	glBindTexture( GL_TEXTURE_2D, material->tex->GetName() );

	drawFunc(tri);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	GL_CheckError("R_RenderPTPass error");
}

void R_RenderShadowMap( drawSurf_t* drawSur, DrawFunc drawFunc ) {
	srfTriangles_t* tri = drawSur->geo;
	material_t* material = drawSur->shaderParms;
	mat4 t = (*drawSur->viewProj) * drawSur->matModel;
	Shader* shader = material->shader;

	glEnableVertexAttribArray(0);

	glUseProgram( shader->GetProgarm() );
	glUniformMatrix4fv( shader->GetUniform(eUniform_MVP), 1, GL_FALSE, &t.m[0] );

	drawFunc(tri);

	glDisableVertexAttribArray(0);
	GL_CheckError("draw common");
}

void R_RenderPhongPass( drawSurf_t* drawSurf, DrawFunc drawFunc ) {
	srfTriangles_t* tri = drawSurf->geo;
	material_t* material = drawSurf->shaderParms;
	mat4 t = (*drawSurf->viewProj) * drawSurf->matModel;
	mat4 modelView = (*drawSurf->view) * drawSurf->matModel;
	mat4 invModelView = modelView.inverse();
	Shader* shader = material->shader;

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	GL_CheckError("R_RenderPhongPass error1111");

	glUseProgram( shader->GetProgarm() );
	glUniformMatrix4fv( shader->GetUniform(eUniform_MVP), 1, GL_FALSE, &t.m[0] );
	glUniformMatrix4fv( shader->GetUniform(eUniform_ModelView), 1, GL_FALSE, &modelView.m[0] );
	glUniformMatrix4fv( shader->GetUniform(eUniform_InvModelView), 1, GL_FALSE, &invModelView.m[0] );

	GL_CheckError("R_RenderPhongPass error");
	glUniform1i( shader->GetUniform(eUniform_Samper0), 0 );
	glBindTexture( GL_TEXTURE_2D, material->tex->GetName() );

	GL_CheckError("R_RenderPhongPass error1");
	glUniform3f(shader->GetUniform(eUniform_EyePos), 0.f, 0.f, 100.f);
	glUniform3f(shader->GetUniform(eUniform_LightPos), -100.f, 100.f, -100.f);

	drawFunc(tri);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	GL_CheckError("R_RenderPhongPass error2");
}

void R_DrawPositon( srfTriangles_t* tri ) {
	glBindBuffer(GL_ARRAY_BUFFER, tri->vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DrawVert), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri->vbo[1]);
	glDrawElements(GL_TRIANGLES, tri->drawCount, GL_UNSIGNED_SHORT, (void*)tri->drawBegin);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void R_DrawPositonTex( srfTriangles_t* tri ) {
	glBindBuffer(GL_ARRAY_BUFFER, tri->vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DrawVert), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(DrawVert), (GLvoid *)12);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri->vbo[1]);
	glDrawElements(GL_TRIANGLES, tri->numIndices, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void R_DrawPositionTexNorm( srfTriangles_t* tri ) {
	glBindBuffer(GL_ARRAY_BUFFER, tri->vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DrawVert), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(DrawVert), (GLvoid *)12);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(DrawVert), (GLvoid *)20);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri->vbo[1]);
	glDrawElements(GL_TRIANGLES, tri->numIndices, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void RB_DrawBounds( Aabb3d* aabb3d ) {
	float vertices[] = {  aabb3d->_min.x, aabb3d->_min.y, aabb3d->_min.z,
						aabb3d->_min.x, aabb3d->_min.y, aabb3d->_max.z,
						aabb3d->_min.x, aabb3d->_max.y, aabb3d->_max.z,
						aabb3d->_min.x, aabb3d->_max.y, aabb3d->_min.z,

						aabb3d->_max.x, aabb3d->_min.y, aabb3d->_min.z,
						aabb3d->_max.x, aabb3d->_min.y, aabb3d->_max.z,
						aabb3d->_max.x, aabb3d->_max.y, aabb3d->_max.z,
						aabb3d->_max.x, aabb3d->_max.y, aabb3d->_min.z,
	};

	unsigned short indices[] = {0, 1, 1, 2, 2, 3, 3, 0,  
							 0, 4, 1, 5, 2, 6, 3, 7, 
							 4, 5, 5, 6, 6, 7, 7, 4};
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_SHORT, indices);
}

void R_RenderBumpPass( drawSurf_t* drawSurf, DrawFunc drawFunc ) {
	srfTriangles_t* tri = drawSurf->geo;
	material_t* material = drawSurf->shaderParms;
	mat4 t = (*drawSurf->viewProj) * drawSurf->matModel;
	mat4 modelView = (*drawSurf->view) * drawSurf->matModel;
	mat4 invModelView = modelView.inverse();
	Shader* shader = material->shader;

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glUseProgram( shader->GetProgarm() );
	glUniformMatrix4fv( shader->GetUniform(eUniform_MVP), 1, GL_FALSE, &t.m[0] );
	glUniformMatrix4fv( shader->GetUniform(eUniform_ModelView), 1, GL_FALSE, &modelView.m[0] );
	glUniformMatrix4fv( shader->GetUniform(eUniform_InvModelView), 1, GL_FALSE, &invModelView.m[0] );

	glActiveTexture(GL_TEXTURE0);
	glUniform1i( shader->GetUniform(eUniform_Samper0), 0 );
	glBindTexture( GL_TEXTURE_2D, material->tex->GetName() );

	glActiveTexture(GL_TEXTURE1);
	glUniform1i( shader->GetUniform(eUniform_BumpMap), 1 );
	glBindTexture( GL_TEXTURE_2D, material->bumpMap->GetName() );

	glUniform3f(shader->GetUniform(eUniform_EyePos), 0.f, 0.f, 10.f);
	glUniform3f(shader->GetUniform(eUniform_LightPos), 0.f, 10.f, 0.f);

	drawFunc(tri);

	glActiveTexture(GL_TEXTURE0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);

	GL_CheckError("draw common");
}

void R_DrawPositonTangent( srfTriangles_t* tri ) {
	glBindBuffer(GL_ARRAY_BUFFER, tri->vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DrawVert), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(DrawVert), (GLvoid *)12);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(DrawVert), (GLvoid *)20);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(DrawVert), (GLvoid *)32);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(DrawVert), (GLvoid *)44);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri->vbo[1]);
	glDrawElements(GL_TRIANGLES, tri->numIndices, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void R_Draw3DCoordinate()
{
	float vertices[] = {0.f, 0.f, 0.f, 
						0.f, 10.f, 0.f,
						0.f, 0.f, 10.f,
						10.f, 0.f, 0.f
	};

	unsigned short indices[] = {0, 1, 0, 2, 0, 3};
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glDrawElements(GL_LINES, 6, GL_UNSIGNED_SHORT, indices);
	GL_CheckError("====");
}

