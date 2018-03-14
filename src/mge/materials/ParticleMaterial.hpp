#ifndef PARTICLEMATERIAL_HPP
#define PARTICLEMATERIAL_HPP

#include "GL/glew.h"
#include "mge/materials/AbstractMaterial.hpp"

class ShaderProgram;
class Texture;
class Particle;

/**
 * This is about the simplest material we can come up with, it demonstrates how to
 * render a single color material without caching, passing in all the matrices we require separately.
 */
class ParticleMaterial : public AbstractMaterial
{
    public:
		ParticleMaterial(Texture* pSpriteSheet, Particle* pParticle);
        virtual ~ParticleMaterial();

        virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix, const GLuint& pShadowMapId) override;

    private:
        //all the static properties are shared between instances of ColorMaterial
        //note that they are all PRIVATE, we do not expose this static info to the outside world
        static ShaderProgram* _shader;
        static void _lazyInitializeShader();

		Texture* _spriteSheet;
		Particle* _particle;
};

#endif // PARTICLEMATERIAL_HPP
