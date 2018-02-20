#include <iostream>
#include "mge/behaviours/GridMovementBehavior.hpp"
#include "mge/eventSystem/SystemEventDispatcher.hpp"
#include "mge/core/GameObject.hpp"

GridMovementBehavior::GridMovementBehavior(float pMoveAmout, bool pActive, int pOnCol, int pOnRow, int pLevelWidth, std::vector<std::string>& pLevelData)
	: AbstractBehaviour(), _moveAmount(pMoveAmout), _active(pActive), _onCol(pOnCol), _onRow(pOnRow), _levelWidth(pLevelWidth) ,_levelData(pLevelData)
{
	SystemEventDispatcher::AddListener(this); 
}


GridMovementBehavior::~GridMovementBehavior()
{
}


void GridMovementBehavior::update(float pStep)
{
	if (!_active)
		return; 
	/*
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !_keyPressed) {
		if (CheckWalkable(Up)) {
			_keyPressed = true; 

			_newPosition.z = _currentPosition.z - _moveAmount;
			//std::cout << "Moving" << _currentPosition;
			std::cout << _newPosition;
			if (_newPosition.z < _currentPosition.z - _moveAmount)
				_newPosition.z = _currentPosition.z - _moveAmount;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !_keyPressed) {
		if (CheckWalkable(Left)) {
			_keyPressed = true;

			_newPosition.x = _currentPosition.x - _moveAmount;
			//std::cout << "Moving" << _currentPosition;
			std::cout << _newPosition;
			if (_newPosition.x < _currentPosition.x - _moveAmount)
				_newPosition.x = _currentPosition.x - _moveAmount;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !_keyPressed) {
		if (CheckWalkable(Down)) {
			_keyPressed = true;

			_newPosition.z = _currentPosition.z + _moveAmount;
			if (_newPosition.z > _moveAmount + _moveAmount)
				_newPosition.z = _currentPosition.z + _moveAmount;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !_keyPressed) {
		std::cout << "Key is being pressed " << std::endl; 
		if (CheckWalkable(Right)) {
			_keyPressed = true; 
			_newPosition.x = _currentPosition.x + _moveAmount;
			//std::cout << "Moving" << _currentPosition;
			//std::cout << _newPosition;
			if (_newPosition.x > _currentPosition.x + _moveAmount)
				_newPosition.x = _currentPosition.x + _moveAmount;
		}
	}
	else {
		_keyPressed = false; 
	}
	if (!_keyPressed)
	{
		_currentPosition = _newPosition;
		_newPosition = glm::vec3(0, 0, 0);
	}
	
	_owner->translate(_currentPosition);
	*/
}

bool GridMovementBehavior::HandleInput(sf::Keyboard::Key pKey)
{
	std::cout << "Hey" + pKey; 

	return true; 
}


void GridMovementBehavior::onNotify(sf::Event pEvent)
{
	switch (pEvent.key.code) {

	case sf::Keyboard::W:
		if (CheckWalkable(Up)) {
			std::cout << "You pressed W "; 
			_newPosition.z = _currentPosition.z + _moveAmount;
			
			_owner->translate(_newPosition);
		}
		_newPosition = glm::vec3(0, 0, 0); 
		break;
	case sf::Keyboard::A:
		if (CheckWalkable(Left)) {
			_keyPressed = true;

			_newPosition.x = _currentPosition.x - _moveAmount;
			//std::cout << "Moving" << _currentPosition;
			_owner->translate(_newPosition);
		}
		_newPosition = glm::vec3(0, 0, 0);
		break; 
	case sf::Keyboard::S:
		if (CheckWalkable(Down)) {
			_keyPressed = true;

			_newPosition.z = _currentPosition.z - _moveAmount;
			_owner->translate(_newPosition);
		}
		_newPosition = glm::vec3(0, 0, 0);
		break;

	case sf::Keyboard::D:
		if (CheckWalkable(Right)) {
			_keyPressed = true;

			_newPosition.x = _currentPosition.x + _moveAmount;
			//std::cout << "Moving" << _currentPosition;
			_owner->translate(_newPosition);
		}
			_newPosition = glm::vec3(0, 0, 0);
			break;
	}

}


bool GridMovementBehavior::CheckWalkable(Direction pDir)
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

	if (pDir == Right)
	{
		std::string desiredPosOnGrid = _levelData[_onCol - 1 + _levelWidth * _onRow];
		std::cout << desiredPosOnGrid << "Row" << _onRow; 
		if (desiredPosOnGrid == "1")
		{
			_onCol--; 
			return true; 
		}
		else {
			return false; 
		}
	}

	if (pDir == Left)
	{
		std::string desiredPosOnGrid = _levelData[_onCol + 1 + _levelWidth * _onRow];
		std::cout << desiredPosOnGrid << " Row" << _onRow; 
		if (desiredPosOnGrid == "1")
		{
			_onCol++;
			return true;
		}
		else {
			return false;
		}
	}

	if (pDir == Up)
	{
		std::string desiredPosOnGrid = _levelData[_onCol + _levelWidth * (_onRow + 1)];
		std::cout << desiredPosOnGrid << " Row " << _onRow; 
		if (desiredPosOnGrid == "1")
		{
			_onRow++; 
			return true;
		}
		else {
			return false;
		}
	}

	if (pDir == Down)
	{
		std::string desiredPosOnGrid = _levelData[_onCol + _levelWidth * (_onRow - 1)];
		std::cout << desiredPosOnGrid << "Col " << _onCol; 
		if (desiredPosOnGrid == "1")
		{
			_onRow--;
			return true;
		}
		else {
			return false;
		}
	}

	int playerPosOnGrid = _onCol + _levelWidth * _onRow; 
	std::cout << "row " << _onRow << " + Level Width * Colum " << _levelWidth << " " << _onCol << std::endl; 
	std::cout << "player thing " << playerPosOnGrid << std::endl; 

	std::cout << _levelData[playerPosOnGrid] << std::endl; 
	return false; 
}

