//SCREEN SHADER VERTEX SHADER
#version 330 core
in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(vertex, 1.0); 
    TexCoords = uv;
} 