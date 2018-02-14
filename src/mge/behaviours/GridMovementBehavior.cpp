#include "mge/behaviours/GridMovementBehavior.hpp"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Keyboard.hpp>


GridMovementBehavior::GridMovementBehavior(int pMoveAmout, bool pActive): AbstractBehaviour(), _moveAmount(pMoveAmout), _active(pActive)
{
}


GridMovementBehavior::~GridMovementBehavior()
{
}

void GridMovementBehavior::update(float pStep)
{
	if (!_active)
		return;

	int moveAmountX = 0;
	int moveAmountY = 0; 
	//std::cout << "stopped Moving" << _currentPosition; 

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !_keyPressed) {
			_newPosition.z = _currentPosition.z - _moveAmount;
			//std::cout << "Moving" << _currentPosition;
			std::cout << _newPosition; 
			if (_newPosition.z <  _currentPosition.z - _moveAmount)
				_newPosition.z = _currentPosition.z - _moveAmount;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !_keyPressed) {
		_newPosition.x = _currentPosition.x - _moveAmount;
		//std::cout << "Moving" << _currentPosition;
		std::cout << _newPosition;
		if (_newPosition.x <  _currentPosition.x - _moveAmount)
			_newPosition.x = _currentPosition.x - _moveAmount;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !_keyPressed) {
		_newPosition.z = _currentPosition.z + _moveAmount;
		if (_newPosition.z >  _moveAmount + _moveAmount)
			_newPosition.z = _currentPosition.z + _moveAmount;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !_keyPressed) {
		_newPosition.x = _currentPosition.x + _moveAmount;
		//std::cout << "Moving" << _currentPosition;
		std::cout << _newPosition;
		if (_newPosition.x > _currentPosition.x + _moveAmount)
			_newPosition.x = _currentPosition.x + _moveAmount;
	}
	else {
		_keyPressed = false; 
		_currentPosition = _newPosition;
		_newPosition = glm::vec3(0, 0, 0);
	}

	_owner->translate(_currentPosition);
}

bool GridMovementBehavior::CheckWalkable()
{
	return false;
}
