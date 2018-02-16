#include "mge/player/Pawn.hpp"
#include "mge/behaviours/GridMovementBehavior.hpp"


Pawn::Pawn(std::string pName, glm::vec3 pPosition): GameObject(pName, pPosition)
{
}


Pawn::~Pawn()
{
}

void Pawn::ChangeState(AbstractMaterial * material)
{
	setMaterial(material);
}

/*
void Pawn::CheckWalkable(Tile pTile)
{
	if (!pTile.walkable)
	{
		Pawn cant move; 
	}
	
}
*/

