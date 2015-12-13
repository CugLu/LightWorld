#ifndef __SHADERSOURCE_H__
#define __SHADERSOURCE_H__


static const char position_vert[] =
"attribute vec4 vPosition;\n"
"uniform mat4 WVP;\n"
"void main() {\n"
"  gl_Position = WVP*vPosition;\n"
"}\n";

static const char position_frag[] =
	"precision mediump float;\n"
	"uniform vec3 COLOR;\n"
	"void main() {\n"
	"   gl_FragColor = vec4(COLOR, 1.0);\n"
	"}\n";
	

//------------------------------------------------------------------------------------------------------
	static const char positiontex_vert[] =
		"attribute vec3 vPosition;\n"
		"attribute vec2 vTexCoord;\n"
		"uniform mat4 WVP;\n"
		"varying vec2 v_texCoord;\n"
		"void main() {\n"
		"  gl_Position = WVP* vec4(vPosition, 1.0);\n"
		"  v_texCoord = vTexCoord;\n"
    "}\n";

	static const char positiontex_frag[] =
		"precision mediump float;\n"
		"uniform sampler2D texture1;"
		"varying vec2 v_texCoord;\n"
		"void main() {\n"
		"   gl_FragColor = texture2D(texture1, v_texCoord);\n"
		//"   gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
		"}\n";
	

#endif // __SHADERSOURCE_H__



