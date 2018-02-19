#include "mge\level\SpecialTile.h"

SpecialTile::SpecialTile(int pColPos, int pRowPos, int pVectorPos) {
	_colPos = pColPos;
	_rowPos = pRowPos;
	_vectorPos = pVectorPos;
}

SpecialTile::~SpecialTile() {

}

int SpecialTile::GetColPos() {
	return _colPos;
}

int SpecialTile::GetRowPos() {
	return _rowPos;
}

int SpecialTile::GetVectorPos() {
	return _vectorPos;
}