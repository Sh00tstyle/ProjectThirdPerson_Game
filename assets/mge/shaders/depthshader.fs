//DEPTH SHADER FRAGMENT SHADER
#version 330 core

in vec2 TexCoord;

uniform sampler2D diffuseTexture;

layout (location = 0) out vec4 FragColor;

void main()
{ 
    FragColor = texture(diffuseTexture, TexCoord);
}