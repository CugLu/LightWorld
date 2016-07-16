// draw common °æ±¾2

#include "draw_common.h"
#include "Shader.h"
#include "Texture.h"
#include "DrawVert.h"
#include "sys/sys_public.h"
#include "Material.h"

void R_BindArrayBuffer(int i) {
	switch (i) {
	case 0: // xyz
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DrawVert), 0);
		break;
	case 1: // st
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(DrawVert), (GLvoid *)12);
		break;
	case 2: // normal
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(DrawVert), (GLvoid *)20);
		break;
	case 3: // tangents
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(DrawVert), (GLvoid *)32);
		break;
	case 4: //
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(DrawVert), (GLvoid *)44);
		break;
	case 5: // color
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(DrawVert), (GLvoid *)56);
		break;
	default:
		Sys_Error("%s %s", __FILE__, __LINE__);
		break;
	}
}

static void R_DrawCommon( srfTriangles_t* tri, unsigned short *attri, unsigned short numAttri ) {
	glBindBuffer(GL_ARRAY_BUFFER, tri->vbo[0]);
	for (int i=0; i<numAttri; i++){
		R_BindArrayBuffer(attri[i]);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri->vbo[1]);
	glDrawElements(GL_TRIANGLES, tri->numIndices, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void R_RenderCommon(drawSurf_t* drawSurf){
	Material* mtr = drawSurf->mtr;
	srfTriangles_t* tri = drawSurf->geo;
	if (mtr == NULL) {
		Sys_Error("draw surf material is null");
		return;
	}

	if (drawSurf->tex == NULL && mtr->HasTexCoord()) {
		Sys_Error("draw surf texture is null");
		return;
	}

	unsigned short* attri = mtr->_attriArr;
	unsigned short numAttri = mtr->_numAttri;
	Shader* shader = &mtr->_shader;

	for (int i = 0; i < numAttri; i++)
		glEnableVertexAttribArray(attri[i]);

	glUseProgram(shader->GetProgarm());

	if (mtr->_hasColor)
		glUniform3f(shader->GetUniform(eUniform_Color), 1.0, 0.0, 0.0);

	if (mtr->_hasTexture) {
		glUniform1i( shader->GetUniform(eUniform_Samper0), 0 );
		glBindTexture( GL_TEXTURE_2D, drawSurf->tex->GetName() );
	}

	//if (mtr->_hasWorldViewPorj)
	//{
	mat4 t = (*drawSurf->viewProj) * drawSurf->matModel;
	glUniformMatrix4fv(shader->GetUniform(eUniform_MVP), 1, GL_FALSE, &t.m[0] );
	//}
	R_DrawCommon(tri, attri, numAttri);

	for (int i = 0; i < numAttri; i++)
		glDisableVertexAttribArray(attri[i]);
}

