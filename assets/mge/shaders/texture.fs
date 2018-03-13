//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1
struct Light {
	int lightType;

	float ambientContribution;

	vec3 lightForward;
	vec3 lightPosition;

	float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;

	float outerConeCos;
	float innerConeCos;

	vec3 lightColor;
};

//light
in vec2 texCoord;
in vec3 worldNormal;
in vec3 worldVertex;

//shadow calculation
in vec4 fragPosLightSpace;

//lights
uniform int lightAmount;
uniform Light lights[32]; //array of max. 32 lights

//shadowmap properties
uniform sampler2D shadowMap;
uniform bool depthRender;

//material properties
uniform sampler2D diffuseTexture;
uniform vec3 ambientColor;
uniform vec3 specularColor;
uniform float shininess;

//camera position
uniform vec3 eyePosition;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

vec3 getLightVector(Light light) {
	vec3 lightVector = light.lightForward; //Directional, only needs light direction

	if(light.lightType == 1 || light.lightType == 2) { //Point and Spot
		lightVector = worldVertex - light.lightPosition; //vectors from lightpos to vertex pos
	}

	return lightVector;
}

float getAttenuation(Light light, vec3 lightVector) {
	float attenuation = 1.0f; //Directional, no attenuation

	if(light.lightType == 1 || light.lightType == 2) { //Point and Spot
		//attenuation
		float lightDistance = length(lightVector);
		attenuation = (light.constantAttenuation + light.linearAttenuation * lightDistance + light.quadraticAttenuation * lightDistance * lightDistance);
	}

	return attenuation;
}

float getSpotEffect(Light light, vec3 lightVector) {
	float spotEffect = 1.0f; //Directional and Point, no spot effect

	if(light.lightType == 2) { //Spot
		//spotlight effect
		float directionCos = dot(normalize(lightVector), normalize(light.lightForward));
		spotEffect = smoothstep(light.outerConeCos, light.innerConeCos, directionCos); //interpolate intensity 

		//smoothstep(min, max, x):
		//returns 0 if x < min, so if we are outside of the outer cone
		//returns 1 if x >= max, so if we are inside of the inner cone
		//otherwise interpolates using Hermite interpolation between 0 and 1
	}

	return spotEffect;
}

vec3 getAmbientTerm(Light light) {
	//ambient
	return ambientColor * light.ambientContribution * texture(diffuseTexture, texCoord).rgb;
}

vec3 getDiffuseTerm(Light light, vec3 lightVector, float attenuation, float spotEffect) {
	//diffuse
	float diffuseIntensity = max (0, dot(normalize(-lightVector), normalize (worldNormal)));
	diffuseIntensity = diffuseIntensity / attenuation * spotEffect; //applying attenuation and spot effect
	
	return diffuseIntensity * light.lightColor * texture(diffuseTexture, texCoord).rgb;
}

vec3 getSpecularTerm(Light light, vec3 lightVector, float attenuation, float spotEffect) {
	//specular
	vec3 reflectedLight = reflect(lightVector, worldNormal); //reflected light ray in the normal (R)
	vec3 eyeVector = eyePosition - worldVertex; //vector from the vertex to the camera/eye position (V)
	float specularIntensity = clamp (dot(normalize(eyeVector), normalize(reflectedLight)), 0, 1);
	specularIntensity = pow (specularIntensity, shininess); //applying shininess
	specularIntensity = specularIntensity / attenuation * spotEffect; //applying attenuation and spot effect

	return specularIntensity * light.lightColor * specularColor;
}

float ShadowCalculation(vec4 fragPosLightSpace) {
	//do shadow stuff
	// perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
	float bias = 0.005;
	//if(lightAmount > 0) bias = max(0.05 * (1.0 - dot(worldNormal, lights[0].lightForward)), 0.005);

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -2; x <= 2; ++x)
	{
		for(int y = -2; y <= 2; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 25.0;

	if(projCoords.z > 1.0) shadow = 0.0;

    return shadow;
}

void main( void ) {
	if(!depthRender) {
		//rendering as normal with the depth map
		vec3 resultColor = texture(diffuseTexture, texCoord).rgb;

		//iterating through the given amount of lights, not the whole array since it might contain unused lights
		if(lightAmount > 0) {
			Light currentLight = lights[0]; //just use our first directional light

			vec3 lightVector = getLightVector(currentLight);
			float attenuation = getAttenuation(currentLight, lightVector);
			float spotEffect = getSpotEffect(currentLight, lightVector);

			vec3 ambientTerm = getAmbientTerm(currentLight);
			vec3 diffuseTerm = getDiffuseTerm(currentLight, lightVector, attenuation, spotEffect);
			vec3 specularTerm = getSpecularTerm(currentLight, lightVector, attenuation, spotEffect);
			
			float shadow = ShadowCalculation(fragPosLightSpace);
			resultColor = (ambientTerm + diffuseTerm/* + specularTerm*/); //adding new values to the old stored ones
			resultColor = resultColor * (1.0f - shadow * 0.5f);
		}

		//cutout
		float alphaCutoff = 0.1f;

		if(texture(diffuseTexture, texCoord).a < alphaCutoff) discard;
		FragColor = vec4(resultColor, 1.0f);

		//filter HDR bright spots (change to 1.0, when using engine lights)
		float brightness = dot(resultColor, vec3(0.2126, 0.7152, 0.0722));
		if(brightness > 1.0f) {
			BrightColor = vec4(resultColor, 1.0f);
		} else {
			BrightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}
	} else {
		//else render depth to shadowmap
		float alphaCutoff = 0.1f;
		if(texture(diffuseTexture, texCoord).a < alphaCutoff) discard;
	}
}