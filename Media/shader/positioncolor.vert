attribute vec3 vPosition;
attribute vec4 vColor;
varying vec4 vFragmentColor;
uniform mat4 WVP;
void main() 
{
	//gl_Position = WVP* vec4(vPosition, 1.0);
	gl_Position = vec4(vPosition, 1.0);
	vFragmentColor = vColor;
}