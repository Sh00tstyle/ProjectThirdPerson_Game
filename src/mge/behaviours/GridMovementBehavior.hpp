#ifndef GRIDMOVEMENTBEHAVIOR_HPP
#define GRIDMOVEMENTBEHAVIOUT_HPP

#include  "mge/behaviours/AbstractBehaviour.hpp"
#include "glm.hpp"
#include <vector>
#include <string>

class GridMovementBehavior : public AbstractBehaviour
{
	public:
		GridMovementBehavior(float pMoveAmount, bool pActive, int pOnCol, int pOnRow, int pLevelWidth, std::vector<std::string>& pLevelData);
		~GridMovementBehavior();
		virtual void update(float pStep);

	private:
		void CheckWalkable(glm::vec3 pDesitedPosition); 

		bool _active; 
		bool _keyPressed = false; 
		float _moveAmount;
		
		int _moveAmountY; 
		int _onCol; 
		int _onRow; 
		int _levelWidth; 
		std::vector<std::string> _levelData; 
		

		glm::vec3 _newPosition; 
		glm::vec3 _currentPosition; 
};

#endif // GRIDMOVEMENTBEHAVIOR
