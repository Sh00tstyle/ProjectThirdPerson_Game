#include "mge/materials/AbstractMaterial.hpp"

AbstractMaterial::AbstractMaterial()
{
    //ctor
}

AbstractMaterial::~AbstractMaterial()
{
    //dtor
}

void AbstractMaterial::renderDepth(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pPerspectiveMatrix) {
}