//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D diffuseTexture;
in vec2 texCoord;
out vec4 fragment_color;

void main( void ) {
	float alphaCutoff = 0.5f;
	vec4 color = texture(diffuseTexture, texCoord);

	if(color.a < alphaCutoff) discard;
	fragment_color = color;
}