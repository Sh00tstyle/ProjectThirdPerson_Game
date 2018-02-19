#include "mge\level\ActivatableTile.h"

ActivatableTile::ActivatableTile(int pColPos, int pRowPos, int pVectorPos, int pID): SpecialTile(pColPos, pRowPos, pVectorPos), _id(pID), _active(false) {

}

ActivatableTile::~ActivatableTile() {

}

void ActivatableTile::Activate() {
	_active = true;

	//TODO: Do some visual stuff like changing material/transparency
}

bool ActivatableTile::IsActive() {
	return _active;
}
