#ifndef TILEBEHAVIOUR_HPP
#define TILEBEHACIOUR_HPP

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "glm.hpp"


class TileBehaviour : public AbstractBehaviour {

	public:
		TileBehaviour(glm::vec3 pTargetPos); 
		~TileBehaviour(); 

		virtual void update(float pStep); 

private:
	void Lerp(glm::vec3 pStartPos, glm::vec3 pEndPos, float pSteps);
	bool _moving; 
	float _startTime; 
	glm::vec3 _targetPos; 
};
#endif // !TILEBEHAVIOUR_HPP
