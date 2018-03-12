//DEPTH SHADER VERTEX SHADER
#version 330 core
in vec3 vertex;
in vec3 normal;
in vec2 uv;

uniform	mat4 projectionMatrix;
uniform	mat4 viewMatrix;
uniform	mat4 modelMatrix;

out vec2 TexCoord;

void main()
{
    gl_Position =  projectionMatrix * viewMatrix * modelMatrix * vec4(vertex, 1);
    TexCoord = uv;
} 