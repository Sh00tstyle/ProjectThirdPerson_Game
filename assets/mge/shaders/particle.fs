//DIFFUSE COLOR FRAGMENT SHADER

#version 330 // for glsl version (12 is for older versions , say opengl 2.1

in vec2 TexCoord;

uniform sampler2D spriteSheet;
uniform float time;

out vec4 FragColor;

void main( void ) {

	vec4 color = texture(spriteSheet, TexCoord);

    float cutout = 0.0f;
    if(color.a < cutout) discard;

    FragColor = color;
}
