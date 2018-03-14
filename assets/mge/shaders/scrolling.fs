//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform float time;

uniform sampler2D diffuseTexture;
in vec2 texCoord;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

void main( void ) {
	//scroll the texture
	vec4 color = texture(diffuseTexture, vec2(texCoord.x + time / 10.0f, texCoord.y));
	FragColor = color;

    /*
    float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0f) {
        BrightColor = vec4(color.rbg, 1.0f);
    } else {
        BrightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    */
}