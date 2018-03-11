#include "glm.hpp"

#include "TextureMaterial.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/config.hpp"

ShaderProgram* TextureMaterial::_shader = NULL;
ShaderProgram* TextureMaterial::_depthShader = NULL;

GLint TextureMaterial::_uMVPMatrix = 0;
GLint TextureMaterial::_uDiffuseTexture = 0;

GLint TextureMaterial::_aVertex = 0;
GLint TextureMaterial::_aNormal = 0;
GLint TextureMaterial::_aUV = 0;

TextureMaterial::TextureMaterial(Texture * pDiffuseTexture):_diffuseTexture(pDiffuseTexture) {
    _lazyInitializeShader();

	_ambientColor = glm::vec3(1, 1, 1); 
	_specularColor = glm::vec3(0.5, 0.5, 0.5);
	_shininess = 2.0f;
}

TextureMaterial::~TextureMaterial() {}

void TextureMaterial::_lazyInitializeShader() {
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH+"texture.vs");
        _shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH+"texture.fs");
        _shader->finalize();

        //cache all the uniform and attribute indexes
        _uMVPMatrix = _shader->getUniformLocation("mvpMatrix");
        _uDiffuseTexture = _shader->getUniformLocation("diffuseTexture");

        _aVertex = _shader->getAttribLocation("vertex");
        _aNormal = _shader->getAttribLocation("normal");
        _aUV =     _shader->getAttribLocation("uv");
    }

	if(!_depthShader) {
		_depthShader = new ShaderProgram();
		_depthShader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "depthshader.vs");
		_depthShader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "depthshader.fs");
		_depthShader->finalize();
	}
}

void TextureMaterial::setDiffuseTexture (Texture* pDiffuseTexture) {
    _diffuseTexture = pDiffuseTexture;
}

void TextureMaterial::render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
    if (!_diffuseTexture) return;

    _shader->use();

	Camera* mainCam = pWorld->getMainCamera(); //needed for specular eye position
	int lightAmount = pWorld->getLightCount();

	//passing in light amount
	glUniform1i(_shader->getUniformLocation("lightAmount"), lightAmount); //passing in the amount of light we have

	//iterate through the lights and add them to the uniform array
	for(int i = 0; i < lightAmount; i++) {
		Light* currentLight = pWorld->getLightAt(i);
		std::string lightString = "lights[" + std::to_string(i) + "]."; //"path" to the right array element

		glUniform1i(_shader->getUniformLocation(lightString + "lightType"), (int)currentLight->getLightType()); //passing in the light type as an int

		glUniform1f(_shader->getUniformLocation(lightString + "ambientContribution"), std::cos(currentLight->getInnerConeAngle()));

		glUniform3fv(_shader->getUniformLocation(lightString + "lightForward"), 1, glm::value_ptr(currentLight->getTransform()[2])); //third row of the lights transform, represents local forward vector
		glUniform3fv(_shader->getUniformLocation(lightString + "lightPosition"), 1, glm::value_ptr(currentLight->getLocalPosition())); //might need to be changed to world pos

		glUniform1f(_shader->getUniformLocation(lightString + "constantAttenuation"), currentLight->getConstantAttenuation());
		glUniform1f(_shader->getUniformLocation(lightString + "linearAttenuation"), currentLight->getLinearAttenuation());
		glUniform1f(_shader->getUniformLocation(lightString + "quadraticAttenuation"), currentLight->getQuadraticAttenuation());

		glUniform1f(_shader->getUniformLocation(lightString + "outerConeCos"), std::cos(currentLight->getOuterConeAngle()));
		glUniform1f(_shader->getUniformLocation(lightString + "innerConeCos"), std::cos(currentLight->getInnerConeAngle()));

		glUniform3fv(_shader->getUniformLocation(lightString + "lightColor"), 1, glm::value_ptr(currentLight->getLightColor() * currentLight->getIntensity())); //applying intensity, before passing it
	}

	//passing all material properties to the shader
	glUniform3fv(_shader->getUniformLocation("ambientColor"), 1, glm::value_ptr(_ambientColor)); //applying contribution, before passing it
	glUniform3fv(_shader->getUniformLocation("specularColor"), 1, glm::value_ptr(_specularColor));
	glUniform1f(_shader->getUniformLocation("shininess"), _shininess);

	//passing in camera position
	glUniform3fv(_shader->getUniformLocation("eyePosition"), 1, glm::value_ptr(mainCam->getWorldPosition())); //might need to be changed to world pos

	//pass in all MVP matrices separately
	glUniformMatrix4fv(_shader->getUniformLocation("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(pProjectionMatrix));
	glUniformMatrix4fv(_shader->getUniformLocation("viewMatrix"), 1, GL_FALSE, glm::value_ptr(pViewMatrix));
	glUniformMatrix4fv(_shader->getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(pModelMatrix));

    //setup texture slot 0
    glActiveTexture(GL_TEXTURE0);
    //bind the texture to the current active slot
    glBindTexture(GL_TEXTURE_2D, _diffuseTexture->getId());
    //tell the shader the texture slot for the diffuse texture is slot 0
    glUniform1i (_uDiffuseTexture, 0);

    //now inform mesh of where to stream its data
    pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);
}

void TextureMaterial::renderDepth(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pPerspectiveMatrix) {
	//if(pWorld->getLightCount() == 0) return;

	_depthShader->use();

	/**
	Light* currentLight = pWorld->getLightAt(0); //hardcoded first light

	glm::vec3 lightPos = currentLight->getLocalPosition();
	glm::vec3 lightDirection = currentLight->getTransform()[2];

	glm::mat4 projectionMat = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
	glm::mat4 viewMat = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 modelMat = glm::mat4(1.0);
	/**/

	glUniformMatrix4fv(_depthShader->getUniformLocation("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(pModelMatrix));
	glUniformMatrix4fv(_depthShader->getUniformLocation("viewMatrix"), 1, GL_FALSE, glm::value_ptr(pViewMatrix));
	glUniformMatrix4fv(_depthShader->getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(pPerspectiveMatrix));

	pMesh->streamToOpenGL(_depthShader->getAttribLocation("vertex"), -1, -1);
}
