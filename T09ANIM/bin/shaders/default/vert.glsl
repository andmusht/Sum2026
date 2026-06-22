/**/
// GLSL version 4.6
#version 460

// variables  locations
layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;

// global variables 
uniform mat4 MatrWVP, MatrW, MatrWInv;
uniform float Time;

// Output parameter (varyng)
out vec4 DrawColor;
out vec3 DrawNormal;
out vec3 DrawPos;
out vec4 DrawPosOrg;
out vec2 DrawTexCoord;
                                      
void main( void )
{
  gl_Position = MatrWVP * vec4(InPosition, 1);
  DrawColor = InColor;
  DrawNormal = mat3(MatrWInv) * InNormal;
  DrawPos = InPosition;  
  DrawPosOrg = vec4(InPosition, 1);
  DrawTexCoord = InTexCoord;
}