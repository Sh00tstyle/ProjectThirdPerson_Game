#include "mge/materials/AbstractMaterial.hpp"

AbstractMaterial::AbstractMaterial()
{
    //ctor
}

AbstractMaterial::~AbstractMaterial()
{
    //dtor
}

void AbstractMaterial::renderDepth(World * pWorld, Mesh * pMesh) {
	//dont do anything
	return;
}

