#ifndef GRIDMOVEMENTBEHAVIOR_HPP
#define GRIDMOVEMENTBEHAVIOUT_HPP

#include  "mge/behaviours/AbstractBehaviour.hpp"
#include "glm.hpp"

class GridMovementBehavior : public AbstractBehaviour
{
	public:
		GridMovementBehavior(int pMoveAmount = 8, bool pActive = true);
		~GridMovementBehavior();
		virtual void update(float pStep);
		bool CheckWalkable(); 

	private:
		bool _active; 
		bool _keyPressed = false; 
		int _moveAmount;
		int _moveAmountY; 

		glm::vec3 _newPosition; 
		glm::vec3 _currentPosition; 
};

#endif // GRIDMOVEMENTBEHAVIOR
