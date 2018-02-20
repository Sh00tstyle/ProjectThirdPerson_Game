#include "mge/player/Pawn.hpp"
#include "mge/behaviours/GridMovementBehavior.hpp"

Pawn::Pawn(std::string pName, glm::vec3 pPosition): GameObject(pName, pPosition)
{
	//_movement = new GridMovementBehavior(pMoveAmount, pActive, pOnCol, pOnRow, pLevelWidth, pLevelData); 
	//setBehaviour(_movement); 
}

Pawn::~Pawn()
{
}

void Pawn::ChangeState(AbstractMaterial * material)
{
	setMaterial(material);
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

