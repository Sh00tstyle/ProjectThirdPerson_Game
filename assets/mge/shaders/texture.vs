//DIFFUSE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

in vec3 vertex;
in vec3 normal;
in vec2 uv;

uniform	mat4 projectionMatrix;
uniform	mat4 viewMatrix;
uniform	mat4 modelMatrix;

uniform mat4 lightProjection;
uniform mat4 lightView;

uniform bool depthRender;

//light calculation
out vec3 worldNormal;
out vec3 worldVertex;

//shadow calculation
out vec4 fragPosLightSpace;

out vec2 texCoord; //make sure the texture coord is interpolated

void main( void ){
	if(!depthRender) {
		//normal MVP for real model
		gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex, 1.0f);

		worldVertex = vec3 (modelMatrix * vec4(vertex, 1.0f)); 
		worldNormal = transpose(inverse(mat3(modelMatrix))) * normal;

		//stuff for shadow calculation
		fragPosLightSpace = lightProjection * lightView * modelMatrix * vec4(vertex, 1.0f);

		texCoord = uv;
	} else {
		//rendering depth
		gl_Position = lightProjection * lightView * modelMatrix * vec4(vertex, 1.0f);
		
		texCoord = uv;
	}
}
