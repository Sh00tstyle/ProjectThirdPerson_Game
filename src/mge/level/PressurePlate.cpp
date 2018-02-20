#include "mge\level\PressurePlate.h"

PressurePlate::PressurePlate(int pColPos, int pRowPos, int pVectorPos, std::string pActivationColor, int pID): SpecialTile(pColPos, pRowPos, pVectorPos), 
_activationColor(pActivationColor), _id(pID) {

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

void PressurePlate::Activate() {
	_targetTile->Activate();
}