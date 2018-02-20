#ifndef GRIDMOVEMENTBEHAVIOR_HPP
#define GRIDMOVEMENTBEHAVIOUT_HPP

#include  "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/eventSystem/SystemEventListener.hpp"
#include "glm.hpp"
#include <vector>
#include <string>
#include <SFML/Window/Keyboard.hpp>

class GridMovementBehavior : public AbstractBehaviour,public SystemEventListener                        
{
	public:
		GridMovementBehavior(float pMoveAmount, bool pActive, int pOnCol, int pOnRow, int pLevelWidth, std::vector<std::string>& pLevelData);
		~GridMovementBehavior();
		virtual void update(float pStep);
		bool HandleInput(sf::Keyboard::Key pKey);

		virtual void onNotify(sf::Event pEvent); 

	private:

		bool _active; 
		bool _keyPressed = false; 
		float _moveAmount;
		
		int _moveAmountY; 
		int _onCol; 
		int _onRow; 
		int _levelWidth; 
		std::vector<std::string> _levelData; 
		
		enum Direction {Up, Left, Down, Right};
		Direction _dir; 

		bool CheckWalkable(Direction pDir); 
		glm::vec3 _newPosition; 
		glm::vec3 _currentPosition; 

		//SystemEventDispatcher _eventDispatcher; 

};

#endif // GRIDMOVEMENTBEHAVIOR
