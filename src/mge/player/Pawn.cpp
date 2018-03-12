#include "mge/player/Pawn.hpp"
#include "mge/behaviours/GridMovementBehavior.hpp"
#include "mge/tileProp.hpp"
#include "mge/managers/ModelManager.h"
#include "mge/audio/AudioContainer.h"

Pawn::Pawn(std::string pName, glm::vec3 pPosition): GameObject(pName, pPosition)
{
	//_movement = new GridMovementBehavior(pMoveAmount, pActive, pOnCol, pOnRow, pLevelWidth, pLevelData); 
	//setBehaviour(_movement); 
}

Pawn::~Pawn()
{
}

void Pawn::ChangeState(std::string value)
{
	if(value == _pawnColor) return; //we are already in the color

	if(value == tileProp::RedColorSwitch) {
		setMaterial(ModelManager::GetRedPlayerMat());
	} else if(value == tileProp::BlueColorSwitch) {
		setMaterial(ModelManager::GetBluePlayerMat());
	}

	_pawnColor = value;
	AudioContainer::PlaySound("CHANGE_COLOR");
}

bool Pawn::CheckWalkableColor(AbstractMaterial * pMaterial)
{
	//No longer necesarry
	//This will get checked in the Scene object
	if (getMaterial() == pMaterial)
		return true; 
	else
	return false;
}

std::string Pawn::getPawnColor() {
	return _pawnColor;
}

void Pawn::setPawnColor(std::string color) {
	_pawnColor = color;
}

/*
void Pawn::onNotify(sf::Event pEvent)
{
	_movement->HandleInput(pEvent.key.code); 
	std::cout << "On Notify is called"; 
}
*/

/*
void Pawn::CheckWalkable(Tile pTile)
{
	if (!pTile.walkable)
	{
		Pawn cant move; 
	}
	
}
*/

