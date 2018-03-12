#include "mge/level/Scene.h" //includes vector, string already
#include <iostream>
#include <sstream>
#include <iterator>

#include "mge/core/World.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/GameObject.hpp"

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"

#include "mge/behaviours/GridMovementBehavior.hpp"

#include "mge/player/Pawn.hpp"
#include "mge/level/SpecialTile.h"
#include "mge/level/PressurePlate.h"
#include "mge/level/ActivatableTile.h"
#include "mge/level/SpawnTile.h"
#include "mge/level/DestinationTile.h"

#include "mge/eventSystem/SystemEventDispatcher.hpp"

#include "mge/audio/AudioContainer.h"

#include "mge/util/tinyxml2.h"
#include "mge/tileProp.hpp"
#include "mge/config.hpp"

#include "mge/managers/ModelManager.h"
#include "mge/managers/SceneManager.h";
#include "mge/UI/UiContainer.h"

Scene::Scene(std::string pFilepath, World* pWorld) {
	_world = pWorld;
	_loadSceneFromFile(pFilepath);
}

Scene::~Scene() {
	//scene cleanup
	_destructScene();

	std::cout << "Destruct Scene" << std::endl;
}

void Scene::ConstructScene(bool showHint) {
	//restore old playfield data in case we reload the scene
	int fieldIndex = 0;
	if (_triesPerHint != 0 && _hintData.size() != 0) fieldIndex = (SceneManager::GetLevelTries() - 1 - UiContainer::GetIgnoCount()) / _triesPerHint; //int division, so no need to floor down

	//activate the hud menu
	if(UiContainer::GetHintsTaken() == 0) {
		//until the first hint
		if(fieldIndex == 0) {
			_playfieldData = _unmodifedPlayfieldData;
		} else if(UiContainer::GetHintActive() && showHint) {
			if(fieldIndex > _hintData.size()) fieldIndex = _hintData.size();
			_playfieldData = _hintData[fieldIndex - 1];
		} else {
			_playfieldData = _unmodifedPlayfieldData;

			UiContainer::SetHintActive(true);
		}
	} else {
		//after the first hint
		if(UiContainer::GetHintActive() && showHint) {
			if(fieldIndex > _hintData.size()) fieldIndex = _hintData.size();
			_playfieldData = _hintData[fieldIndex - 1];
		} else {
			if(UiContainer::GetHintsTaken() < fieldIndex && UiContainer::GetHintsTaken() < _hintData.size()) UiContainer::SetHintActive(true);

			if(UiContainer::GetHintsTaken() > _hintData.size()) fieldIndex = _hintData.size();
			else fieldIndex = UiContainer::GetHintsTaken();
			_playfieldData = _hintData[fieldIndex - 1];
		}
	}
	

	//Make the scene from there
	for(unsigned i = 0; i < _playfieldData.size(); i++) {
		std::string tileProperty = _playfieldData[i];

		if(tileProperty == tileProp::None) {
			_tileObjects.push_back(nullptr); //adding in nothing to keep the 2d array illusion
			continue; //skip it when we dont have to build anything
		}

		//treating the vector as a 2d array
		int col = i % _levelWidth;
		int row = i / _levelWidth;

		float tileSize = 2.0f; //should be seamless, since every plane is 2 units wide

		float xPos = -(col - _levelWidth / 2.0f) * tileSize - 1;
		float zPos = (row - _levelHeight / 2.0f) * tileSize + 1;

		//setting up the player
		if(tileProperty == tileProp::PlayerSpawn) {
			_pawn = new Pawn("pawn", glm::vec3(xPos, 0, zPos));
			_pawn->setMesh(ModelManager::GetPlayerMesh());

			//coloring tile and player
			if(_spawnTile->GetStartingColor() == tileProp::RedTile) {
				_pawn->ChangeState(tileProp::RedColorSwitch);
			} else {
				_pawn->ChangeState(tileProp::BlueColorSwitch);
			}

			_pawn->setBehaviour(new GridMovementBehavior(tileSize, true,col, row,  *this));
			_world->add(_pawn);
		}

		//Create Tile
		GameObject* newTile = new GameObject("tile" + std::to_string(i), glm::vec3(xPos, 0, zPos));

		_setTileMesh(newTile, tileProperty);
		_setTileMaterial(newTile, tileProperty, xPos, zPos, i);

		_tileObjects.push_back(newTile);
		_world->add(newTile);
	}

	//create scene objects
	for(unsigned i = 0; i < _sceneObjects.size(); i++) {
		GameObject* myObj = _sceneObjects[i];
		myObj->setMesh(_sceneObjectMeshes[i]);
		myObj->setMaterial(_sceneObjectMats[i]);
		_world->add(myObj);
	}

	for(unsigned i = 0; i < _activatableTiles.size(); i++) {
		_activatableTiles[i]->Reset();
	}

	AudioContainer::PlaySound("START_LEVEL");

	std::cout << "Constructed Scene" << std::endl;
}

void Scene::RemoveScene() {
	SystemEventDispatcher::RemoveListener("MovementListener");

	//reset all activatable tiles
	for(unsigned i = 0; i < _activatableTiles.size(); i++) {
		ActivatableTile* myTile = _activatableTiles[i];
		myTile->Reset();
	}

	//remove tiles from the scene
	for(unsigned i = 0; i < _tileObjects.size(); i++) {
		GameObject* myObj = _tileObjects[i];
		if(myObj != nullptr) _world->remove(myObj);
	}

	_tileObjects.clear();

	//remove the player from the scene
	if(_pawn != nullptr) _world->remove(_pawn);
	_pawn = nullptr;

	//remove environment from the scene
	for(unsigned i = 0; i < _sceneObjects.size(); i++) {
		GameObject* myObj = _sceneObjects[i];
		if(myObj != nullptr)_world->remove(myObj);
	}
}

int Scene::GetLevelWidth() {
	return _levelWidth;
}

int Scene::GetLevelHeight() {
	return _levelHeight;
}

int Scene::GetPlayfieldDataCount() {
	return _playfieldData.size();
}

std::string Scene::GetPlayfieldValue(int vectorIndex) {
	return _playfieldData[vectorIndex];
}

std::string Scene::GetPlayfieldValue(int colIndex, int rowIndex) {
	
	return _playfieldData[colIndex  + rowIndex * _levelWidth];
}

glm::vec3 Scene::GetTileWorldPos(int colIndex, int rowIndex, float pTileSize)
{
	return glm::vec3(-(colIndex - _levelWidth/2.0f) * pTileSize,0, (rowIndex - _levelWidth / 2.0f) * pTileSize);
}

void Scene::SetPlayfieldColor(int vectorIndex, std::string value) {
	_playfieldData[vectorIndex] = value;
	//Assign new random material based on the new color
	if(value == tileProp::RedTile) {
		_tileObjects[vectorIndex]->setMaterial(ModelManager::GetRndRedTileMat());
	} else if(value == tileProp::BlueTile) {
		_tileObjects[vectorIndex]->setMaterial(ModelManager::GetRndBlueTileMat());
	}
}

void Scene::SetPlayfieldColor(int colIndex, int rowIndex, std::string value) {
	_playfieldData[colIndex + rowIndex * _levelWidth] = value;

	if(value == tileProp::RedTile) {
		_tileObjects[colIndex + rowIndex * _levelWidth]->setMaterial(ModelManager::GetRndRedTileMat());
	} else if(value == tileProp::BlueTile) {
		_tileObjects[colIndex + rowIndex * _levelWidth]->setMaterial(ModelManager::GetRndBlueTileMat());
	}
}

void Scene::SetPawnColor(std::string value)
{
	_pawn->ChangeState(value);
}

std::string Scene::GetPawnColor()
{
	if (_pawn->getMaterial() == ModelManager::GetRedPlayerMat())
		return tileProp::RedTile; 
	else if (_pawn->getMaterial() == ModelManager::GetBluePlayerMat())
		return tileProp::BlueTile; 
}

std::string Scene::GetDestinationColor()
{
	return _destinationTile->GetNeededColor(); 
}

std::string Scene::GetStartTileColor()
{
	return _spawnTile->GetStartingColor(); 
}

PressurePlate* Scene::GetPressurePlate(int pCol, int pRow)
{
	for (unsigned i = 0; i < _pressurePlates.size(); i++)
		if (_pressurePlates[i]->CheckPositionOnGrid(pCol, pRow))
			return _pressurePlates[i]; 
}

ActivatableTile * Scene::GetActivatableTile(int pCol, int pRow)
{
	for (unsigned i = 0; i < _activatableTiles.size(); i++)
		if (_activatableTiles[i]->CheckPositionOnGrid(pCol, pRow))
			return _activatableTiles[i];
}

GameObject * Scene::GetTileObject(int col, int row) {
	if(col + row * _levelWidth >= _tileObjects.size()) return nullptr;
	else return _tileObjects[col + row * _levelWidth];
}

void Scene::_loadSceneFromFile(std::string filepath) {
	//this method reads the XML file and fills all varibales with data

	const char * charPath = filepath.c_str(); //converting the input string to a char so tinyxml can read it

    //Reading the xml level file
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError eResult = doc.LoadFile(charPath);
	if(eResult != tinyxml2::XML_SUCCESS) std::cout << "Failed to load xml file" << std::endl;

	tinyxml2::XMLNode* root = doc.FirstChild(); //first root that will always be random information generated by unity
	root = root->NextSibling(); //actual first root (PlayfieldContainer)

	//playfield
	tinyxml2::XMLElement* element = root->FirstChildElement("Playfield");
	if(element == nullptr) std::cout << "Null in Playfield" << std::endl;

	//storing the read width and height as a reference in our ints that are provided from the outside
	element->QueryIntAttribute("Height", &_levelHeight);
	element->QueryIntAttribute("Width", &_levelWidth);

	const char* readChar = element->Attribute("Content"); //read list in the element of "Playfield" as char
	std::string arrayString(readChar); //converting char into a string
	std::istringstream iss(arrayString);

	//vector of chars that represent tile values, split arrayString by " "
	std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

	//storing a copies of the result vector
	_playfieldData = results;
	_unmodifedPlayfieldData = results;

	//hint playfields
	element = root->FirstChildElement("HintPlayfields");
	if(element == nullptr) std::cout << "Null in HintPlayfields" << std::endl;

	tinyxml2::XMLElement* listElement = element->FirstChildElement("HintPlayfield");

	while(listElement != nullptr) {
		readChar = listElement->Attribute("Content"); //read list in the element of "HintPlayfield" as char
		std::string hintString(readChar); //converting char into a string
		std::istringstream iss(hintString);

		//vector of chars that represent tile values, split arrayString by " "
		std::vector<std::string> resultVector((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

		//store it (yes, the whole vector gets stored in a vector)
		_hintData.push_back(resultVector);

		listElement = listElement->NextSiblingElement("HintPlayfield");
	}

	//hint props
	element = root->FirstChildElement("HintProps");
	if(element == nullptr) std::cout << "Null in HintProps" << std::endl;

	element->QueryIntAttribute("TriesPerHint", &_triesPerHint);

	//scene objects
	element = root->FirstChildElement("SceneObjects"); //gp to the next element
	if(element == nullptr) std::cout << "Null in Scene Objects" << std::endl;

	listElement = element->FirstChildElement("SceneObject");
	
	while(listElement != nullptr) {
		//name
		readChar = listElement->Attribute("Name");
		std::string nameString(readChar);

		//pos
		float xPos;
		float yPos;
		float zPos;

		listElement->QueryFloatAttribute("xPos", &xPos);
		listElement->QueryFloatAttribute("yPos", &yPos);
		listElement->QueryFloatAttribute("zPos", &zPos);

		//rotation
		float xRot;
		float yRot;
		float zRot;
		float wRot;

		listElement->QueryFloatAttribute("xRot", &xRot);
		listElement->QueryFloatAttribute("yRot", &yRot);
		listElement->QueryFloatAttribute("zRot", &zRot);
		listElement->QueryFloatAttribute("wRot", &wRot);

		//scale
		float xScale;
		float yScale;
		float zScale;

		listElement->QueryFloatAttribute("xScale", &xScale);
		listElement->QueryFloatAttribute("yScale", &yScale);
		listElement->QueryFloatAttribute("zScale", &zScale);

		//model
		readChar = listElement->Attribute("Model");
		std::string modelString(readChar);

		//texture
		readChar = listElement->Attribute("Texture");
		std::string textureString(readChar);

		//create mesh and material for the gameobjects
		Mesh* objMesh = Mesh::load(config::MGE_MODEL_PATH + modelString);
		AbstractMaterial* objMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + textureString));
		GameObject* newGameObject = new GameObject(nameString, glm::vec3(xPos, yPos, zPos));
		
		glm::quat rotation = glm::quat(wRot, xRot, yRot, zRot);

		//apply rotation and scale
		newGameObject->rotate(glm::angle(rotation), glm::axis(rotation));
		newGameObject->scale(glm::vec3(xScale, yScale, zScale));

		_sceneObjectMeshes.push_back(objMesh);
		_sceneObjectMats.push_back(objMat);
		_sceneObjects.push_back(newGameObject);

		listElement = listElement->NextSiblingElement("SceneObject");
	}

	//pressure plates
	element = root->FirstChildElement("PressurePlates"); //gp to the next element
	if(element == nullptr) std::cout << "Null in PressurePlates" << std::endl;

	listElement = element->FirstChildElement("PressurePlate");

	int redColorCount = 0;
	int blueColorCount = 0;

	while(listElement != nullptr) {
		int colPos;
		int rowPos;
		int id;

		listElement->QueryIntAttribute("ColPos", &colPos);
		listElement->QueryIntAttribute("RowPos", &rowPos);
		listElement->QueryIntAttribute("ID", &id);

		readChar = listElement->Attribute("NeededColor");
		std::string colorString(readChar);

		PressurePlate* newPlate;
		
		if(colorString == tileProp::RedTile) {
			newPlate = new PressurePlate(colPos, rowPos, colPos + rowPos * _levelWidth, colorString, id, redColorCount);
			redColorCount++;
		} else if(colorString == tileProp::BlueTile) {
			newPlate = new PressurePlate(colPos, rowPos, colPos + rowPos * _levelWidth, colorString, id, blueColorCount);
			blueColorCount++;
		}

		_pressurePlates.push_back(newPlate);

		listElement = listElement->NextSiblingElement("PressurePlate");
	}

	//activatable tiles
	element = root->FirstChildElement("ActivatableTiles"); //go to the next element
	if(element == nullptr) std::cout << "Null in ActivatableTiles" << std::endl;

	listElement = element->FirstChildElement("ActivatableTile");

	redColorCount = 0;
	blueColorCount = 0;

	while(listElement != nullptr) {
		int colPos;
		int rowPos;
		int id;

		listElement->QueryIntAttribute("ColPos", &colPos);
		listElement->QueryIntAttribute("RowPos", &rowPos);
		listElement->QueryIntAttribute("ID", &id);

		ActivatableTile* newActivatable = new ActivatableTile(colPos, rowPos, colPos + rowPos * _levelWidth, id, this);

		for(unsigned i = 0; i < _pressurePlates.size(); i++) {
			if(_pressurePlates[i]->GetID() == id) {
				_pressurePlates[i]->SetTargetTile(newActivatable); //assign the tile with the respective id to the pressure plate
				newActivatable->SetIndex(_pressurePlates[i]->GetIndex());
				newActivatable->SetColor(_pressurePlates[i]->GetActivationColor());
				break; //end loop here since we found our item
			}
		}

		_activatableTiles.push_back(newActivatable);

		listElement = listElement->NextSiblingElement("ActivatableTile");
	}

	//spawn tile
	element = root->FirstChildElement("SpawnTile");
	if(element == nullptr) std::cout << "Null in SpawnTile" << std::endl;

	int spawnCol;
	int spawnRow;

	element->QueryIntAttribute("ColPos", &spawnCol);
	element->QueryIntAttribute("RowPos", &spawnRow);

	readChar = element->Attribute("StartingColor");
	std::string startColorString(readChar);

	_spawnTile = new SpawnTile(spawnCol, spawnRow, spawnCol + spawnRow * _levelWidth, startColorString);

	//destination tile
	element = root->FirstChildElement("DestinationTile");
	if(element == nullptr) std::cout << "Null in DestinationTile" << std::endl;

	int destCol;
	int destRow;

	element->QueryIntAttribute("ColPos", &destCol);
	element->QueryIntAttribute("RowPos", &destRow);

	readChar = element->Attribute("NeededColor");
	std::string neededColorString(readChar);

	_destinationTile = new DestinationTile(destCol, destRow, destCol + destRow * _levelWidth, neededColorString);

	std::cout << "Read Scene from " + filepath << std::endl;
}

void Scene::_setTileMaterial(GameObject* newTile, std::string tileProperty, float xPos, float zPos, int i) {
	if(tileProperty == tileProp::Uncolored) {
		newTile->setMaterial(ModelManager::GetRndUncoloredMat());
	} else if(tileProperty == tileProp::PlayerSpawn) {
		if(_spawnTile->GetStartingColor() == tileProp::RedTile) {
			newTile->setMaterial(ModelManager::GetRndRedTileMat());
		} else {
			newTile->setMaterial(ModelManager::GetRndBlueTileMat());
		}
	} else if(tileProperty == tileProp::RedTile) {
		newTile->setMaterial(ModelManager::GetRndRedTileMat());
	} else if(tileProperty == tileProp::BlueTile) {
		newTile->setMaterial(ModelManager::GetRndBlueTileMat());
	} else if(tileProperty == tileProp::Destination) {
		if(_destinationTile->GetNeededColor() == tileProp::RedTile) newTile->setMaterial(ModelManager::GetRedDestinationMat());
		else newTile->setMaterial(ModelManager::GetBlueDestinationMat());
	} else if(tileProperty == tileProp::PressurePlate) {
		for(unsigned j = 0; j < _pressurePlates.size(); j++) {
			PressurePlate* myPlate = _pressurePlates[j];

			if(myPlate->GetVectorPos() == i) {
				if(myPlate->GetActivationColor() == tileProp::RedTile) newTile->setMaterial(ModelManager::GetRedPressurePlateMat(myPlate->GetIndex())); //replace 0 with the correcct texture index
				else newTile->setMaterial(ModelManager::GetBluePressurePlateMat(myPlate->GetIndex())); //replace 0 with the correcct texture index
			}
		}
	} else if(tileProperty == tileProp::ActivatableTile) {
		for(unsigned j = 0; j < _pressurePlates.size(); j++) {
			PressurePlate* myPlate = _pressurePlates[j];
			ActivatableTile* myTile = myPlate->GetTargetTile();

			if(myTile->GetVectorPos() == i) {
				if(myPlate->GetActivationColor() == tileProp::RedTile) newTile->setMaterial(ModelManager::GetRedActivatableTileInactiveMat(myTile->GetIndex())); //replace 0 with the correcct texture index
				else newTile->setMaterial(ModelManager::GetBlueActivatableTileInactiveMat(myTile->GetIndex())); //replace 0 with the correcct texture index
			}
		}
	} else if(tileProperty == tileProp::RedColorSwitch) {
		newTile->setMaterial(ModelManager::GetRedColorSwitchMat());
	} else if(tileProperty == tileProp::BlueColorSwitch) {
		newTile->setMaterial(ModelManager::GetBlueColorSwitchMat());
	}
}

void Scene::_setTileMesh(GameObject * newTile, std::string tileProperty) {
	//sett mesh of the tile based on the property/type
	if(tileProperty == tileProp::Destination) {
		newTile->setMesh(ModelManager::GetDestinationTileMesh());
	} else if(tileProperty == tileProp::PressurePlate) {
		newTile->setMesh(ModelManager::GetPressurePlateMesh());
	} else if(tileProperty == tileProp::ActivatableTile) {
		newTile->setMesh(ModelManager::GetActivatableTileMesh());
	} else if(tileProperty == tileProp::BlueColorSwitch || tileProperty == tileProp::RedColorSwitch) {
		newTile->setMesh(ModelManager::GetColorSwitchMesh());
	} else {
		newTile->setMesh(ModelManager::GetNormalTileMesh());
	}
}

void Scene::_destructScene() {
	//remove the player
	_world->remove(_pawn);
	delete _pawn;

	//remove all tiles
	for(unsigned i = 0; i < _tileObjects.size(); i++) {
		if(_tileObjects[i] == nullptr) continue;

		_world->remove(_tileObjects[i]);
		delete _tileObjects[i];
	}

	//remove all scene objects
	for(unsigned i = 0; i < _sceneObjects.size(); i++) {
		if(_sceneObjects[i] == nullptr) continue;

		_world->remove(_sceneObjects[i]);
		delete _sceneObjects[i];
	}

	//remove all scene object meshes
	for(unsigned i = 0; i < _sceneObjectMeshes.size(); i++) {
		if(_sceneObjectMeshes[i] == nullptr) continue;

		delete _sceneObjectMeshes[i];
	}

	//remove all scene object materials
	for(unsigned i = 0; i < _sceneObjectMats.size(); i++) {
		if(_sceneObjectMats[i] == nullptr) continue;

		delete _sceneObjectMats[i];
	}

	//remove all pressure plates
	for(unsigned i = 0; i < _pressurePlates.size(); i++) {
		if(_pressurePlates[i] == nullptr) continue;

		delete _pressurePlates[i];
	}

	//remove all activatable tiles
	for(unsigned i = 0; i < _activatableTiles.size(); i++) {
		if(_activatableTiles[i] == nullptr) continue;

		delete _activatableTiles[i];
	}

	//remove spawn and destination tile
	delete _spawnTile;
	delete _destinationTile;
}
