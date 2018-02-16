#include <iostream>
#include "mge/behaviours/GridMovementBehavior.hpp"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Keyboard.hpp>


GridMovementBehavior::GridMovementBehavior(float pMoveAmout, bool pActive, int pOnCol, int pOnRow, int pLevelWidth, std::vector<std::string>& pLevelData)
	: AbstractBehaviour(), _moveAmount(pMoveAmout), _active(pActive), _onCol(pOnCol), _onRow(pOnRow), _levelWidth(pLevelWidth) ,_levelData(pLevelData)
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

	CheckWalkable();
		_active = false; 

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


void GridMovementBehavior::CheckWalkable(glm::vec3 pDesiredPosition)
{
	//read in the levelData and check if the desired position to the 
	//value on the grid and return if that is walkable or not

	/*
		0 = None
		1 = Uncolored
		2 = Pawn Spawn Pos + uncolored tile
		3 = Red Tile
		4 = Blue Tile
		5 = Destination
		6 = PressurePlate
		7 = ActivatableTile
		8 = RedColorSwitch (the one that changes the color for the pawn)
		9 = BlueColorSwitch
	*/
	int playPosOnGrid = _onCol + _levelWidth * _onRow; 
	std::cout << "player thing" << playPosOnGrid; 


}

