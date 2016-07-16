uniform vec3 fvLightPosition;
uniform vec3 fvEyePosition;
uniform mat4 WVP;
uniform mat4 modelView;
uniform mat4 invModelView;

attribute vec3 vPosition;
attribute vec2 vTexCoord;
attribute vec3 vNormal;

varying vec3 Normal;
varying vec2 TexCoord;
varying vec3 ViewDirection;
varying vec3 LightDirection;
   
void main()
{
   gl_Position = WVP*vec4(vPosition, 1.0);
   TexCoord = vTexCoord;
    
   vec4 fvObjectPosition = modelView * vec4(vPosition, 1.0);
   vec3 pos = vec3(fvObjectPosition) / fvObjectPosition.w;

   ViewDirection  = pos.xyz - fvEyePosition;
   LightDirection = pos.xyz - fvLightPosition;
   Normal         = vec3(invModelView * vec4(vNormal, 0.0));
   
}