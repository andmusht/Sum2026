/**/

#version 330
 
layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;
 
uniform mat4 MatrWVP, MatrW, MatrWInv;

out vec4 DrawColor;
out vec3 DrawNormal;
out vec4 DrawPos;
out vec4 DrawPosOrg;
                                      
void main( void )
{
  gl_Position = MatrWVP * vec4(InPosition, 1);
  DrawColor = InColor;
  DrawNormal = mat3(MatrWInv) * InNormal;
  //DrawPos = (MatrW * vec4(InPosition, 1)).xyz;
  //DrawPosOrg = InPosition;
}