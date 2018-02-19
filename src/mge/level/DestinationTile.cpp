#include "mge\level\DestinationTile.h"

DestinationTile::DestinationTile(int pColPos, int pRowPos, int pVectorPos, std::string pNeededColor):
	SpecialTile(pColPos, pRowPos, pVectorPos), _neededColor(pNeededColor) {

}

DestinationTile::~DestinationTile() {

}

std::string DestinationTile::GetNeededColor() {
	return _neededColor;
}
