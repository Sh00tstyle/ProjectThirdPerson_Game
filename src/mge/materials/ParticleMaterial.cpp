#include "glm.hpp"

#include "ParticleMaterial.hpp"
#include "mge/config.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/core/Texture.hpp"
#include "mge/level/Particle.h"

ShaderProgram* ParticleMaterial::_shader = NULL;

ParticleMaterial::ParticleMaterial(Texture* pSpriteSheet, Particle* pParticle):_spriteSheet (pSpriteSheet)
{
	_particle = pParticle;
    //every time we create an instance of colormaterial we check if the corresponding shader has already been loaded
    _lazyInitializeShader();
}

void ParticleMaterial::_lazyInitializeShader() {
    //this shader contains everything the material can do (it can render something in 3d using a single color)
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH+"particle.vs");
        _shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH+"particle.fs");
        _shader->finalize();
    }
}

ParticleMaterial::~ParticleMaterial()
{
    //dtor
}

void ParticleMaterial::render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix, const GLuint& pShadowMapId) {
	if(_spriteSheet == nullptr) return;

    _shader->use();

    //pass in all MVP matrices separately
    glUniformMatrix4fv ( _shader->getUniformLocation("projectionMatrix"),   1, GL_FALSE, glm::value_ptr(pProjectionMatrix));
    glUniformMatrix4fv ( _shader->getUniformLocation("viewMatrix"),         1, GL_FALSE, glm::value_ptr(pViewMatrix));
    glUniformMatrix4fv ( _shader->getUniformLocation("modelMatrix"),        1, GL_FALSE, glm::value_ptr(pModelMatrix));

	//pass in time to animate
	glUniform1f(_shader->getUniformLocation("time"), glm::float1(std::clock() / 10000.f));

	//pass in particle properties

	//pass in diffuse texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _spriteSheet->getId());
	glUniform1i(_shader->getUniformLocation("spriteSheet"), 0);

    //now inform mesh of where to stream its data
    pMesh->streamToOpenGL(
        _shader->getAttribLocation("vertex"),
        _shader->getAttribLocation("normal"),
        _shader->getAttribLocation("uv")
    );

}
