#include "mge\level\PressurePlate.h"
#include "mge/audio/AudioContainer.h"
PressurePlate::PressurePlate(int pColPos, int pRowPos, int pVectorPos, std::string pActivationColor, int pID, int pIndex): SpecialTile(pColPos, pRowPos, pVectorPos), 
_activationColor(pActivationColor), _id(pID), _index(pIndex) {
	_col = pColPos; 
	_row = pRowPos; 
}

PressurePlate::~PressurePlate() {

}

std::string PressurePlate::GetActivationColor() {
	return _activationColor;
}

int PressurePlate::GetID() {
	return _id;
}

void PressurePlate::SetTargetTile(ActivatableTile * target) {
	_targetTile = target;
}

ActivatableTile * PressurePlate::GetTargetTile() {
	return _targetTile;
}

bool PressurePlate::CheckPositionOnGrid(int pCol, int pRow)
{
	if (_col == pCol && _row == pRow)
		return true;
	else
		return false;
}

void PressurePlate::Activate() {
	AudioContainer::PlaySound("ACTIVATE_PLATE");

	_targetTile->Activate();
}

int PressurePlate::GetIndex() {
	return _index;
}
