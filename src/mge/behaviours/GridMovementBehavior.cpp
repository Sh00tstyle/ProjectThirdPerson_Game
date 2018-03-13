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
#include "mge/audio/AudioContainer.h"
#include "mge/UI/UiContainer.h"
#include "mge/util/Calculate.hpp"

GridMovementBehavior::GridMovementBehavior(float pMoveAmout, bool pActive, int pCol, int pRow, Scene& pScene)
	: AbstractBehaviour(), _moveAmount(pMoveAmout), _active(pActive), _onCol(pCol), _onRow(pRow) ,_scene(pScene)
{
	SystemEventDispatcher::AddListener(this,"MovementListener"); 
	_startTime = clock() / 100.0f;
}

GridMovementBehavior::~GridMovementBehavior()
{
	SystemEventDispatcher::RemoveListener("MovementListener");
}

void GridMovementBehavior::update(float pStep)
{
	//std::cout << "The current world possition " << _owner->getWorldPosition() << std::endl; 
	if (_moving) {

		 _speed = ((clock() / 100.0f) - _startTime) * 20.0f * pStep;
		
		std::cout << "speed " << _speed << std::endl; 

		if (_speed > 1) {
			_currentTile = _targetTile;
			_startTime = clock() / 100.0f;
			_speed = 0; 
			_moving = false; 
		}

		if (_speed < 0)
		{
			_startTime = clock() / 100.0f;
			_speed = 1; 
		}
		_owner->setLocalPosition(Calculate::Lerp(_currentTile, _targetTile, _speed)); 
	}
	//if certain key is pressed move in a direction 
}

void GridMovementBehavior::onNotify(sf::Event pEvent)
{
	if(!_moving) {
		_startTime = clock() / 100.0f;
		_speed = 0;
		Move(pEvent.key.code);
	}
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
	if (pKey == sf::Keyboard::W)
	{
		if(CheckWalkableTile(_onCol, _onRow - 1)) {
			glm::mat4 targetTrans;
			targetTrans = glm::rotate(targetTrans, glm::radians(0.0f), glm::vec3(0, 1, 0));
			_owner->setTransform(targetTrans);
			//_targetTile = _scene.GetTileWorldPos(_onCol, _onRow - 1, _moveAmount);
			_targetTile = _currentTile - glm::vec3(0,0,_moveAmount);
			
			//_startTime = clock() / 100.0f;
			//_speed = 0;

			_onRow--;
			_moving = true;

			AudioContainer::PlaySound("MOVE_SNAIL");
		} else {
			AudioContainer::PlaySound("BLOCK_SNAIL");
		}
	}

	if (pKey == sf::Keyboard::A)
	{
		if(CheckWalkableTile(_onCol + 1, _onRow)) {
			glm::mat4 targetTrans;
			//targetTrans = glm::translate(targetTrans, _owner->getWorldPosition());
			targetTrans = glm::rotate(targetTrans, glm::radians(90.0f), glm::vec3(0, 1, 0));
			_owner->setTransform(targetTrans);
			_onCol++;
			//_targetTile = _scene.GetTileWorldPos(_onCol + 1, _onRow, _moveAmount);

			//_startTime = clock() / 100.0f;
			//_speed = 0;

			
			_targetTile = _currentTile - glm::vec3(_moveAmount, 0, 0);
			_moving = true;

			AudioContainer::PlaySound("MOVE_SNAIL");
		} else {
			AudioContainer::PlaySound("BLOCK_SNAIL");
		}
	}

	if (pKey == sf::Keyboard::S)
	{
		if(CheckWalkableTile(_onCol, _onRow + 1)) {
			glm::mat4 targetTrans;
			//targetTrans = glm::translate(targetTrans, _owner->getWorldPosition());
			targetTrans = glm::rotate(targetTrans, glm::radians(180.0f), glm::vec3(0, 1, 0));
			_onRow++;
			_owner->setTransform(targetTrans);
			_targetTile = _currentTile + glm::vec3(0, 0, _moveAmount);
			//_targetTile = _scene.GetTileWorldPos(_onCol , _onRow + 1, _moveAmount);
			//_startTime = clock() / 100.0f;
			//_speed = 0;

			
			_moving = true;

			AudioContainer::PlaySound("MOVE_SNAIL");
		} else {
			AudioContainer::PlaySound("BLOCK_SNAIL");
		}	
	}

	if (pKey == sf::Keyboard::D)
	{
		if(CheckWalkableTile(_onCol - 1, _onRow)) {
			glm::mat4 targetTrans;
			//targetTrans = glm::translate(targetTrans, _owner->getWorldPosition());
			targetTrans = glm::rotate(targetTrans, glm::radians(270.0f), glm::vec3(0, 1, 0));
			_owner->setTransform(targetTrans);
			_onCol--;
			//_targetTile = _scene.GetTileWorldPos(_onCol - 1, _onRow, _moveAmount);

			_targetTile = _currentTile + glm::vec3(_moveAmount, 0, 0);
			//_startTime = clock() / 100.0f;
			//_speed = 0;

			
			_moving = true;

			AudioContainer::PlaySound("MOVE_SNAIL");
		} else {
			AudioContainer::PlaySound("BLOCK_SNAIL");
		}
		
		
	}
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
		_scene.SetPawnColor(tileProp::RedColorSwitch);
		return true; 
	}
	else if (_scene.GetPlayfieldValue(pCol, pRow) == tileProp::BlueColorSwitch) {
		_scene.SetPawnColor(tileProp::BlueColorSwitch);
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
			AudioContainer::PlaySound("END_LEVEL");
			UiContainer::SelectMenu("LEVEL " + std::to_string(SceneManager::GetLevelNumber()));
			UiContainer::DisplayStars(SceneManager::GetLevelNumber());

			return true;
		}
	}
	
	return false;
}

