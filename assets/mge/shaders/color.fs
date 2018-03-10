//DIFFUSE COLOR FRAGMENT SHADER

#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform vec3 diffuseColor;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

void main( void ) {
	FragColor = vec4 (diffuseColor,1);
	BrightColor = vec4(diffuseColor.rgb * 0.09, 1.0f);

	/**
    float brightness = dot(diffuseColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0f) {
        BrightColor = vec4(diffuseColor.rbg, 1.0f);
    } else {
        BrightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
	/**/
}
