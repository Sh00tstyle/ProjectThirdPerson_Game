#include "mge\level\ActivatableTile.h"
#include "mge\level\Scene.h"
#include "mge\managers\ModelManager.h"
#include "mge\tileProp.hpp"
#include "mge/audio/AudioContainer.h"

ActivatableTile::ActivatableTile(int pColPos, int pRowPos, int pVectorPos, int pID, Scene* pScene): SpecialTile(pColPos, pRowPos, pVectorPos), _id(pID), _scene(pScene), _active(false) {
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
	if(_active) return;
	
	_active = true;
	AudioContainer::PlaySound("ACTIVATE_PLATE");
	GameObject* tile = _scene->GetTileObject(_col, _row);

	if(tile == nullptr) return;

	/**
	if(_color == tileProp::RedTile) {
		tile->setMaterial(ModelManager::GetRedActivatableTileActiveMat(_index));
	} else if(_color == tileProp::BlueTile) {
		tile->setMaterial(ModelManager::GetBlueActivatableTileActiveMat(_index));
	}
	/**/

	if(tile->getLocalPosition().y == -0.5f) {
		tile->translate(glm::vec3(0, 0.5f, 0));
		AudioContainer::PlaySound("TILE_MOVE");
	}
}

void ActivatableTile::Reset() {
	_active = false;

	GameObject* tile = _scene->GetTileObject(_col, _row);

	if(tile == nullptr) return;

	/**
	if(_color == tileProp::RedTile) {
		tile->setMaterial(ModelManager::GetRedActivatableTileInactiveMat(_index));
	} else if(_color == tileProp::BlueTile) {
		tile->setMaterial(ModelManager::GetBlueActivatableTileInactiveMat(_index));
	}
	/**/

	if(tile->getLocalPosition().y == 0.0f) tile->translate(glm::vec3(0, -0.5f, 0));
}

bool ActivatableTile::IsActive() {
	return _active;
}

void ActivatableTile::SetColor(std::string color) {
	_color = color;
}

void ActivatableTile::SetIndex(int index) {
	_index = index;
}

int ActivatableTile::GetIndex() {
	return _index;
}
