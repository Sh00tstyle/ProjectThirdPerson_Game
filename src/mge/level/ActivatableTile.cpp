#include "mge\level\ActivatableTile.h"

ActivatableTile::ActivatableTile(int pColPos, int pRowPos, int pVectorPos, int pID): SpecialTile(pColPos, pRowPos, pVectorPos), _id(pID), _active(false) {
	_col = pColPos; 
	_row = pRowPos;
}

ActivatableTile::~ActivatableTile() {

}

bool ActivatableTile::CheckPositionOnGrid(int pCol, int pRow)
{
	if (_col == pCol && _row == pRow)
		return true;
	else
		return false;
}

void ActivatableTile::Activate() {
	
	_active = true;

	//TODO: Do some visual stuff like changing material/transparency
}

void ActivatableTile::Reset() {
	_active = false;

	//TODO: Do some visual stuff like changing material/transparency
}

bool ActivatableTile::IsActive() {
	return _active;
}
