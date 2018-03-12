#include "TileBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/util/Calculate.hpp"

TileBehaviour::TileBehaviour(glm::vec3 pTargetPos) : AbstractBehaviour(), _targetPos(pTargetPos)
{
	_startTime = clock() / 100.0f; 
}

TileBehaviour::~TileBehaviour()
{
}

void TileBehaviour::update(float pStep)
{
	float speed = ((clock() / 100.0f) - _startTime) * pStep * 2.0f;

	if (speed > 1) {
		_startTime = clock() / 100.0f;
		speed = 0;
	}

	if (speed < 0)
	{
		_startTime = clock() / 100.0f;
		speed = 1;
	}

	_owner->setLocalPosition(Calculate::Lerp(_owner->getWorldPosition(), _targetPos, speed));
}

void TileBehaviour::Lerp(glm::vec3 pStartPos, glm::vec3 pEndPos, float pSteps)
{
	glm::vec3 delta = pStartPos - pEndPos; 
	glm::vec3 moveSteps = delta / 8; 

	if (_owner->getWorldPosition() != pEndPos)
	{
		_owner->setLocalPosition(_owner->getWorldPosition() + moveSteps); 
	}
	else {
		_moving = false; 
	}

}
