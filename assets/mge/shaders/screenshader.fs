//SCREEN SHADER FRAGMENT SHADER
#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform bool usePP;

uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;

uniform float exposure;

void main()
{ 
    if(usePP) {
        const float gamma = 1.0;
        vec3 hdrColor = texture(screenTexture, TexCoords).rgb;      
        vec3 bloomColor = texture(bloomTexture, TexCoords).rgb;
        hdrColor += bloomColor; // additive blending
        // tone mapping
        vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
        // also gamma correct while we're at it       
        result = pow(result, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    } else {
        FragColor = texture(screenTexture, TexCoords);
    }
}