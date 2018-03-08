#include <iostream>
#include "mge/behaviours/GridMovementBehavior.hpp"
#include "mge/eventSystem/SystemEventDispatcher.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/tileProp.hpp"
#include "mge/level/Scene.h"
#include "mge/level/PressurePlate.h"
#include "mge/level/ActivatableTile.h"
#include "mge/managers/SceneManager.h"
#include "mge/managers/InputManager.h"
#include "mge/UI/UiContainer.h"


GridMovementBehavior::GridMovementBehavior(float pMoveAmout, bool pActive, int pCol, int pRow, Scene& pScene)
	: AbstractBehaviour(), _moveAmount(pMoveAmout), _active(pActive), _onCol(pCol), _onRow(pRow) ,_scene(pScene)
{
	SystemEventDispatcher::AddListener(this,"MovementListener"); 
	

}

GridMovementBehavior::~GridMovementBehavior()
{
	SystemEventDispatcher::RemoveListener("MovementListener");
}


void GridMovementBehavior::update(float pStep)
{
	
	if (_moving) {
		SmoothMove(_currentTile, _targetTile, 8); 
	}
	//if certain key is pressed move in a direction 
}


void GridMovementBehavior::onNotify(sf::Event pEvent)
{
	if(!_moving)
	Move(pEvent.key.code);
}

void GridMovementBehavior::Move(sf::Keyboard::Key pKey)
{
	if(!InputManager::GetGameInput()) return;

	_currentTile = _owner->getWorldPosition();
	/*
	 tileLeft = _scene.GetPlayfieldValue(_onCol + 1, _onRow);
	 tileRight = _scene.GetPlayfieldValue(_onCol - 1, _onRow);
	 tileUp = _scene.GetPlayfieldValue(_onCol, _onRow - 1);
	 tileDown = _scene.GetPlayfieldValue(_onCol, _onRow + 1);
	*/

	if (!_moving)
		_targetTile = _owner->getWorldPosition(); 

	if (pKey == sf::Keyboard::W  && CheckWalkableTile(_onCol, _onRow - 1))
	{
		
		glm::mat4 targetTrans;
		targetTrans = glm::translate(targetTrans, _owner->getWorldPosition());
		targetTrans = glm::rotate(targetTrans, glm::radians(0.0f), glm::vec3(0, 1, 0));
		_owner->setTransform(targetTrans);
		_onRow--;
		_targetTile.z = _currentTile.z - _moveAmount;
		_moving = true; 
	}

	if (pKey == sf::Keyboard::A && CheckWalkableTile(_onCol + 1, _onRow))
	{
		
		glm::mat4 targetTrans;
		targetTrans = glm::translate(targetTrans, _owner->getWorldPosition());
		targetTrans = glm::rotate(targetTrans, glm::radians(90.0f), glm::vec3(0, 1, 0));
		_owner->setTransform(targetTrans);
		_onCol++;
		_targetTile.x = _currentTile.x - _moveAmount;
		_moving = true;
	}

	if (pKey == sf::Keyboard::S  && CheckWalkableTile(_onCol, _onRow + 1))
	{
		
		glm::mat4 targetTrans;
		targetTrans = glm::translate(targetTrans, _owner->getWorldPosition());
		targetTrans = glm::rotate(targetTrans, glm::radians(180.0f), glm::vec3(0, 1, 0));
		_owner->setTransform(targetTrans);
		_onRow++; 
		_targetTile.z = _currentTile.z + _moveAmount;
		_moving = true;
	}

	if (pKey == sf::Keyboard::D && CheckWalkableTile(_onCol - 1, _onRow))
	{

		
		glm::mat4 targetTrans;
		targetTrans = glm::translate(targetTrans, _owner->getWorldPosition()); 
		targetTrans = glm::rotate(targetTrans, glm::radians(270.0f), glm::vec3(0, 1, 0));
		_owner->setTransform(targetTrans);
		_onCol--; 
		_targetTile.x = _currentTile.x + _moveAmount;
		_moving = true;
	}
}

bool GridMovementBehavior::CheckWalkable(Direction pDir)
{
	return true; 
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

	
	

	if (pDir == Right)
	{
		if (CheckWalkableTile(tileRight, _onCol - 1, _onRow))
		{
			_onCol--; 
			return true; 
		}
		else
		{
			return false; 
		}
		/*
		if (_onCol > 0) {
			std::string desiredPosOnGrid = _scene.GetPlayfieldValue(_onCol - 1, _onRow);
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
		
	}

	if (pDir == Left)
	{
		if (CheckWalkableTile(tileLeft, _onCol + 1, _onRow))
		{
			_onCol++;
			return true; 
		}
		else
		{
			return false;
		}
		
		if (_onCol < _scene.GetLevelWidth() - 1) {
			std::string desiredPosOnGrid = _scene.GetPlayfieldValue(_onCol + 1, _onRow);
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
		
	}

	if (pDir == Up)
	{
		if (CheckWalkableTile(tileUp, _onCol, _onRow - 1))
		{
			_onRow--; 
			return true; 
		}
		else
		{
			return false;
		}
		
		if (_onRow > 0) {

			std::string desiredPosOnGrid = _scene.GetPlayfieldValue(_onCol, _onRow - 1);
			std::cout << desiredPosOnGrid << " Row " << _onRow;
			if (desiredPosOnGrid == "1")
			{
				_onRow--;
				return true;
			}
			if (desiredPosOnGrid == "7")
			{
				_onRow++;
			}
			else {
				return false;
			}
		}
		
	}

	if (pDir == Down)
	{
		if (CheckWalkableTile(tileRight, _onCol, _onRow + 1))
		{
			_onRow++;
			return true; 
		}
		else
		{
			return false;
		}
		
		if (_onRow < _scene.GetLevelWidth() - 1) {
			std::string desiredPosOnGrid = _scene.GetPlayfieldValue(_onCol, _onRow + 1);
			std::cout << desiredPosOnGrid << "Row " << _onRow;
			if (desiredPosOnGrid == "1")
			{
				_onRow++;
				return true;
			}
			else {
				return false;
			}
		}
		
	}

	return false; 
	*/
}

bool GridMovementBehavior::CheckWalkableTile(int pCol, int pRow)
{

	if (pCol < 0 || pCol >= _scene.GetLevelWidth()) {
		std::cout << "You went too far " << std::endl; 
		return false; 
	}
	if (pRow < 0 || pRow >= _scene.GetLevelHeight()) {
		std::cout << "You went too far " << std::endl;
		return false; 
	}

	/*
	std::string tileLeft = _scene.GetPlayfieldValue(_onCol + 1, _onRow); 
	std::string tileRight = _scene.GetPlayfieldValue(_onCol - 1, _onRow); 
	std::string tileUp = _scene.GetPlayfieldValue(_onCol, _onRow - 1); 
	std::string tileDown = _scene.GetPlayfieldValue(_onCol, _onRow + 1); 
	*/

	if (_scene.GetPlayfieldValue(pCol, pRow) == tileProp::Uncolored) {
		_scene.SetPlayfieldColor(pCol, pRow, _scene.GetPawnColor());
		return true;
	}
	else if (_scene.GetPlayfieldValue(pCol, pRow) == tileProp::RedTile && _scene.GetPawnColor() == tileProp::RedTile)
		return true;
	else if (_scene.GetPlayfieldValue(pCol, pRow) == tileProp::BlueTile && _scene.GetPawnColor() == tileProp::BlueTile)
		return true;
	else if (_scene.GetPlayfieldValue(pCol, pRow) == tileProp::RedColorSwitch) {
		 _scene.SetPawnColor(_scene.GetPlayfieldValue(pCol, pRow));
		return true; 
	}
	else if (_scene.GetPlayfieldValue(pCol, pRow) == tileProp::BlueColorSwitch) {
		_scene.SetPawnColor(_scene.GetPlayfieldValue(pCol, pRow));
		return true;
	}
	else if (_scene.GetPlayfieldValue(pCol, pRow) == tileProp::PressurePlate)
	{
		_pressurePlate = _scene.GetPressurePlate(pCol, pRow);  
		if (_pressurePlate->GetActivationColor() == _scene.GetPawnColor()) {
			_pressurePlate->Activate();
			return true;
		}
		else
			return false; 
	}
	else if (_scene.GetPlayfieldValue(pCol, pRow) == tileProp::ActivatableTile)
	{
		_activatableTile = _scene.GetActivatableTile(pCol, pRow);
		if (_activatableTile->IsActive()) {
			_scene.SetPlayfieldColor(pCol, pRow, _scene.GetPawnColor()); 
			return true;
		}
		else
			return false;
	}
	else if (_scene.GetPlayfieldValue(pCol, pRow) == tileProp::PlayerSpawn && _scene.GetStartTileColor() == _scene.GetPawnColor())
	{
		return true; 
	}
	else if (_scene.GetPlayfieldValue(pCol, pRow) == tileProp::Destination)
	{
		if (_scene.GetDestinationColor() == _scene.GetPawnColor())
		{
			UiContainer::SelectMenu("LEVEL " + std::to_string(SceneManager::GetLevelNumber()));
			//SceneManager::LoadNextScene();

			return true;
		}
	}

	/*
	if (pTile == tileProp::RedColorSwitch && _pawnColor == tileProp::BlueColorSwitch)
	{
		_scene.SetPawnColor(pTile);
		return true;
	}
	else if (pTile == tileProp::RedColorSwitch && _pawnColor != tileProp::BlueColorSwitch) {
		return true; 
	}
	if (pCol < 0 || pCol > _scene.GetLevelWidth() - 1 || pRow < 0 || pRow > _scene.GetLevelHeight() - 1)
	{
		return false; 
	}
	*/
	//if (pTile == tileProp::Destination)
	return false;
}

void GridMovementBehavior::SmoothMove(glm::vec3 pStartTile, glm::vec3 pEndTile, float pSpeed)
{
	glm::vec3 delta = pEndTile - pStartTile; 
	glm::vec3 moveSteps = delta / pSpeed; 

	if (_owner->getWorldPosition() != pEndTile)
	{
		//_owner->translate(moveSteps); 
		_owner->setLocalPosition(_owner->getWorldPosition() + moveSteps);
	}
	else
	{
		_moving = false; 
	}
}


