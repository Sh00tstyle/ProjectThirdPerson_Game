#include "mge/behaviours/AbstractBehaviour.hpp"
#include "SFML/Graphics.hpp"

#pragma once

class ParticleBehaviour : public AbstractBehaviour
{
public :
	ParticleBehaviour(std::string pFileName, int pWidth, int pHeight);
	~ParticleBehaviour();

	virtual void update(float pSteps);
};