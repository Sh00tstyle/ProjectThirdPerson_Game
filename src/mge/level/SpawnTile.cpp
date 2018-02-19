#include "mge\level\SpawnTile.h"

SpawnTile::SpawnTile(int pColPos, int pRowPos, int pVectorPos, std::string pStartingColor):
	SpecialTile(pColPos, pRowPos, pVectorPos), _startingColor(pStartingColor) {

}

SpawnTile::~SpawnTile() {

}

std::string SpawnTile::GetStartingColor() {
	return _startingColor;
}
