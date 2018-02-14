#include "mge/player/Pawn.hpp"



Pawn::Pawn(std::string pName, glm::vec3 pPosition): GameObject(pName, pPosition)
{
}


Pawn::~Pawn()
{
}

void Pawn::SetState(AbstractMaterial * material)
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

