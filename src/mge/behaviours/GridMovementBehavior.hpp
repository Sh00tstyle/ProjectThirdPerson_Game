#ifndef GRIDMOVEMENTBEHAVIOR_HPP
#define GRIDMOVEMENTBEHAVIOUT_HPP

#include  "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/eventSystem/SystemEventListener.hpp"
#include "glm.hpp"
#include <vector>
#include <string>
#include <SFML/Window/Keyboard.hpp>

class Scene; 
class PressurePlate; 
class Destination; 
class ActivatableTile; 

class GridMovementBehavior : public AbstractBehaviour, public SystemEventListener                        
{
	public:
		GridMovementBehavior(float pMoveAmount, bool pActive, int pCol, int pRown ,Scene& pScene);
		~GridMovementBehavior();
		virtual void update(float pStep);

		virtual void onNotify(sf::Event pEvent); 

	private:

		bool _active; 
		float _moveAmount;

		int _moveAmountY; 
		int _onCol; 
		int _onRow; 
		Scene& _scene; 
		PressurePlate* _pressurePlate; 
		ActivatableTile* _activatableTile; 

		enum Direction {Up, Left, Down, Right};
		Direction _dir; 

		void Move(sf::Keyboard::Key pKey); 
		bool CheckWalkable(Direction pDir);
		bool CheckWalkableTile(int pCol, int pRow);
		glm::vec3 _newPosition; 
		glm::vec3 _currentPosition; 

		//SystemEventDispatcher _eventDispatcher; 

};

#endif // GRIDMOVEMENTBEHAVIOR
