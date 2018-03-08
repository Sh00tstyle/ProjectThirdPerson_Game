//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform float time;

uniform sampler2D diffuseTexture;
in vec2 texCoord;
out vec4 fragment_color;

void main( void ) {
	//scroll the texture
	vec4 color = texture(diffuseTexture, vec2(texCoord.x + time / 25.0f, texCoord.y));
	fragment_color = color;
}