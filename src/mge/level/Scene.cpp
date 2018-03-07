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

#include "mge/util/tinyxml2.h"
#include "mge/tileProp.hpp"
#include "mge/config.hpp"

Scene::Scene(std::string pFilepath, World* pWorld) {
	_world = pWorld;
	_loadSceneFromFile(pFilepath);
}

Scene::~Scene() {
	//scene cleanup
	_destructScene();

	std::cout << "Destruct Scene" << std::endl;
}

void Scene::ConstructScene() {
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

		float xPos = -(col - _levelWidth / 2.0f) * tileSize;
		float zPos = (row - _levelHeight / 2.0f) * tileSize;

		//setting up the player
		if(tileProperty == tileProp::PlayerSpawn) {
			_pawn = new Pawn("pawn", glm::vec3(xPos, 0, zPos));
			_pawn->setMesh(_playerMesh);

			//coloring tile and player
			if(_spawnTile->GetStartingColor() == tileProp::RedTile) {
				_pawn->setMaterial(_redPlayerMat);
			} else {
				_pawn->setMaterial(_bluePlayerMat);
			}

			_pawn->setBehaviour(new GridMovementBehavior(tileSize, true,col, row,  *this));
			_world->add(_pawn);
		}

		//Create Tile
		GameObject* newTile = new GameObject("tile" + std::to_string(i), glm::vec3(xPos, 0, zPos));
		newTile->setMesh(_tileMesh);

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

	std::cout << "Constructed Scene" << std::endl;
}

void Scene::RemoveScene() {
	//restore old playfield data in case we reload the scene
	_playfieldData = _unmodifedPlayfieldData;
	SystemEventDispatcher::RemoveListener("MovementListener");

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

	//reset all activatable tiles
	for(unsigned i = 0; i < _activatableTiles.size(); i++) {
		ActivatableTile* myTile = _activatableTiles[i];
		myTile->Reset();
	}

	//reset some other properties
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
		_tileObjects[vectorIndex]->setMaterial(_redTileMats[rand() % _redTileMats.size()]);
	} else if(value == tileProp::BlueTile) {
		_tileObjects[vectorIndex]->setMaterial(_blueTileMats[rand() % _blueTileMats.size()]);
	}
}

void Scene::SetPlayfieldColor(int colIndex, int rowIndex, std::string value) {
	_playfieldData[colIndex + rowIndex * _levelWidth] = value;

	if(value == tileProp::RedTile) {
		_tileObjects[colIndex + rowIndex * _levelWidth]->setMaterial(_redTileMats[rand() % _redTileMats.size()]);
	} else if(value == tileProp::BlueTile) {
		_tileObjects[colIndex + rowIndex * _levelWidth]->setMaterial(_blueTileMats[rand() % _blueTileMats.size()]);
	}
}

void Scene::SetPawnColor(std::string value)
{
	if (value == tileProp::RedColorSwitch) {
		_pawn->ChangeState(_redPlayerMat); 
	}
	else if (value == tileProp::BlueColorSwitch) {
		_pawn->ChangeState(_bluePlayerMat); 
	}
}

std::string Scene::GetPawnColor()
{
	if (_pawn->getMaterial() == _redPlayerMat)
		return tileProp::RedTile; 
	else if (_pawn->getMaterial() == _bluePlayerMat)
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

	//scene objects
	element = root->FirstChildElement("SceneObjects"); //gp to the next element
	if(element == nullptr) std::cout << "Null in Scene Objects" << std::endl;

	tinyxml2::XMLElement* listElement = element->FirstChildElement("SceneObject");
	
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

	while(listElement != nullptr) {
		int colPos;
		int rowPos;
		int id;

		listElement->QueryIntAttribute("ColPos", &colPos);
		listElement->QueryIntAttribute("RowPos", &rowPos);
		listElement->QueryIntAttribute("ID", &id);

		readChar = listElement->Attribute("NeededColor");
		std::string colorString(readChar);

		PressurePlate* newPlate = new PressurePlate(colPos, rowPos, colPos + rowPos * _levelWidth, colorString, id);
		_pressurePlates.push_back(newPlate);

		listElement = listElement->NextSiblingElement("PressurePlate");
	}

	//activatable tiles
	element = root->FirstChildElement("ActivatableTiles"); //go to the next element
	if(element == nullptr) std::cout << "Null in ActivatableTiles" << std::endl;

	listElement = element->FirstChildElement("ActivatableTile");

	while(listElement != nullptr) {
		int colPos;
		int rowPos;
		int id;

		listElement->QueryIntAttribute("ColPos", &colPos);
		listElement->QueryIntAttribute("RowPos", &rowPos);
		listElement->QueryIntAttribute("ID", &id);

		ActivatableTile* newActivatable = new ActivatableTile(colPos, rowPos, colPos + rowPos * _levelWidth, id);

		for(unsigned i = 0; i < _pressurePlates.size(); i++) {
			if(_pressurePlates[i]->GetID() == id) _pressurePlates[i]->SetTargetTile(newActivatable); //assign the tile with the respective id to the pressure plate
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

	//tile model
	element = root->FirstChildElement("TileModel");
	if(element == nullptr) std::cout << "Null in TileModel" << std::endl;

	readChar = element->Attribute("ModelFile");
	std::string tileModelFileString(readChar);

	_tileMesh = Mesh::load(config::MGE_MODEL_PATH + tileModelFileString);

	//player model
	element = root->FirstChildElement("PlayerModel");
	if(element == nullptr) std::cout << "Null in PlayerModel" << std::endl;

	readChar = element->Attribute("ModelFile");
	std::string playerModelFileString(readChar);

	_playerMesh = Mesh::load(config::MGE_MODEL_PATH + playerModelFileString);

	//red player texture
	element = root->FirstChildElement("RedPlayerTexture");
	if(element == nullptr) std::cout << "Null in RedPlayerTexture" << std::endl;

	readChar = element->Attribute("TextureFile");
	std::string redPlayerTex(readChar);

	_redPlayerMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + redPlayerTex));

	//blue player texture
	element = root->FirstChildElement("BluePlayerTexture");
	if(element == nullptr) std::cout << "Null in BluePlayerTexture" << std::endl;

	readChar = element->Attribute("TextureFile");
	std::string bluePlayerTex(readChar);

	_bluePlayerMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + bluePlayerTex));

	//uncolored tile textures
	element = root->FirstChildElement("UncoloredTileTextures");
	if(element == nullptr) std::cout << "Null in UncoloredTileTextures" << std::endl;

	listElement = element->FirstChildElement("UncoloredTileTexture");

	while(listElement != nullptr) {
		readChar = listElement->Attribute("TextureFile");
		std::string uncoloredTexture(readChar);

		AbstractMaterial* newMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + uncoloredTexture));
		_uncoloredTileMats.push_back(newMat);

		listElement = listElement->NextSiblingElement("UncoloredTileTexture");
	}

	//red tile textures
	element = root->FirstChildElement("RedTileTextures");
	if(element == nullptr) std::cout << "Null in RedTileTextures" << std::endl;

	listElement = element->FirstChildElement("RedTileTexture");

	while(listElement != nullptr) {
		readChar = listElement->Attribute("TextureFile");
		std::string redTexture(readChar);

		AbstractMaterial* newMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + redTexture));
		_redTileMats.push_back(newMat);

		listElement = listElement->NextSiblingElement("RedTileTexture");
	}

	//blue tile textures
	element = root->FirstChildElement("BlueTileTextures");
	if(element == nullptr) std::cout << "Null in BlueTileTextures" << std::endl;

	listElement = element->FirstChildElement("BlueTileTexture");

	while(listElement != nullptr) {
		readChar = listElement->Attribute("TextureFile");
		std::string blueTexture(readChar);

		AbstractMaterial* newMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + blueTexture));
		_blueTileMats.push_back(newMat);

		listElement = listElement->NextSiblingElement("BlueTileTexture");
	}

	//red destination texture
	element = root->FirstChildElement("RedDestinationTileTexture");
	if(element == nullptr) std::cout << "Null in RedDestinationTileTexture" << std::endl;

	readChar = element->Attribute("TextureFile");
	std::string redDestinationTileTex(readChar);

	_redDestinationMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + redDestinationTileTex));

	//blue destination texture
	element = root->FirstChildElement("BlueDestinationTileTexture");
	if(element == nullptr) std::cout << "Null in BlueDestinationTileTexture" << std::endl;

	readChar = element->Attribute("TextureFile");
	std::string blueDestinationTileTex(readChar);

	_blueDestinationMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + blueDestinationTileTex));

	//red pressure plate textures
	element = root->FirstChildElement("RedPressurePlateTextures");
	if(element == nullptr) std::cout << "Null in RedPressurePlateTextures" << std::endl;

	listElement = element->FirstChildElement("RedPressurePlateTexture");

	while(listElement != nullptr) {
		readChar = listElement->Attribute("TextureFile");
		std::string redTexture(readChar);

		AbstractMaterial* newMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + redTexture));
		_redPressurePlateMats.push_back(newMat);

		listElement = listElement->NextSiblingElement("RedPressurePlateTexture");
	}

	//blue pressure plate textures
	element = root->FirstChildElement("BluePressurePlateTextures");
	if(element == nullptr) std::cout << "Null in BluePressurePlateTextures" << std::endl;

	listElement = element->FirstChildElement("BluePressurePlateTexture");

	while(listElement != nullptr) {
		readChar = listElement->Attribute("TextureFile");
		std::string blueTexture(readChar);

		AbstractMaterial* newMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + blueTexture));
		_bluePressurePlateMats.push_back(newMat);

		listElement = listElement->NextSiblingElement("BluePressurePlateTexture");
	}

	//red activatable tile textures
	element = root->FirstChildElement("RedActivatableTileTextures");
	if(element == nullptr) std::cout << "Null in RedActivatableTileTextures" << std::endl;

	listElement = element->FirstChildElement("RedActivatableTileTexture");

	while(listElement != nullptr) {
		readChar = listElement->Attribute("TextureFile");
		std::string redTexture(readChar);

		AbstractMaterial* newMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + redTexture));
		_redActivatableMats.push_back(newMat);

		listElement = listElement->NextSiblingElement("RedActivatableTileTexture");
	}

	//blue activatable tile textures
	element = root->FirstChildElement("BlueActivatableTileTextures");
	if(element == nullptr) std::cout << "Null in BlueActivatableTileTextures" << std::endl;

	listElement = element->FirstChildElement("BlueActivatableTileTexture");

	while(listElement != nullptr) {
		readChar = listElement->Attribute("TextureFile");
		std::string blueTexture(readChar);

		AbstractMaterial* newMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + blueTexture));
		_blueActivatableMats.push_back(newMat);

		listElement = listElement->NextSiblingElement("BlueActivatableTileTexture");
	}

	//red color switch texture
	element = root->FirstChildElement("RedColorSwitchTexture");
	if(element == nullptr) std::cout << "Null in RedColorSwitchTexture" << std::endl;

	readChar = element->Attribute("TextureFile");
	std::string redColorSwitchTex(readChar);

	_redColorSwitchMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + redColorSwitchTex));

	//blue color switch texture
	element = root->FirstChildElement("BlueColorSwitchTexture");
	if(element == nullptr) std::cout << "Null in BlueColorSwitchTexture" << std::endl;

	readChar = element->Attribute("TextureFile");
	std::string blueColorSwitchTex(readChar);

	_blueColorSwitchMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + blueColorSwitchTex));

	std::cout << "Read Scene from " + filepath << std::endl;
}

void Scene::_setTileMaterial(GameObject* newTile, std::string tileProperty, float xPos, float zPos, int i) {
	int rnd = 0;

	if(tileProperty == tileProp::Uncolored) {
		rnd = rand() % (_uncoloredTileMats.size());
		newTile->setMaterial(_uncoloredTileMats[rnd]);
	} else if(tileProperty == tileProp::PlayerSpawn) {
		if(_spawnTile->GetStartingColor() == tileProp::RedTile) {
			rnd = rand() % (_redTileMats.size());
			newTile->setMaterial(_redTileMats[rnd]);
		} else {
			rnd = rand() % (_blueTileMats.size());
			newTile->setMaterial(_blueTileMats[rnd]);
		}
	} else if(tileProperty == tileProp::RedTile) {
		rnd = rand() % (_redTileMats.size());
		newTile->setMaterial(_redTileMats[rnd]);
	} else if(tileProperty == tileProp::BlueTile) {
		rnd = rand() % (_blueTileMats.size());
		newTile->setMaterial(_blueTileMats[rnd]);
	} else if(tileProperty == tileProp::Destination) {
		if(_destinationTile->GetNeededColor() == tileProp::RedTile) newTile->setMaterial(_redDestinationMat);
		else newTile->setMaterial(_blueDestinationMat);
	} else if(tileProperty == tileProp::PressurePlate) {
		for(unsigned j = 0; j < _pressurePlates.size(); j++) {
			PressurePlate* myPlate = _pressurePlates[j];

			if(myPlate->GetVectorPos() == i) {
				if(myPlate->GetActivationColor() == tileProp::RedTile) newTile->setMaterial(_redPressurePlateMats[0]); //replace 0 with the correcct texture index
				else newTile->setMaterial(_bluePressurePlateMats[0]); //replace 0 with the correcct texture index
			}
		}
	} else if(tileProperty == tileProp::ActivatableTile) {
		for(unsigned j = 0; j < _pressurePlates.size(); j++) {
			PressurePlate* myPlate = _pressurePlates[j];
			ActivatableTile* myTile = myPlate->GetTargetTile();

			if(myTile->GetVectorPos() == i) {
				if(myPlate->GetActivationColor() == tileProp::RedTile) newTile->setMaterial(_redActivatableMats[0]); //replace 0 with the correcct texture index
				else newTile->setMaterial(_blueActivatableMats[0]); //replace 0 with the correcct texture index
			}
		}
	} else if(tileProperty == tileProp::RedColorSwitch) {
		newTile->setMaterial(_redColorSwitchMat);
	} else if(tileProperty == tileProp::BlueColorSwitch) {
		newTile->setMaterial(_blueColorSwitchMat);
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

	//remove player and tile mesh
	delete _playerMesh;
	delete _tileMesh;

	//remove player mats
	delete _redPlayerMat;
	delete _bluePlayerMat;

	//remove uncolored tile mats
	for(unsigned i = 0; i < _uncoloredTileMats.size(); i++) {
		if(_uncoloredTileMats[i] == nullptr) continue;

		delete _uncoloredTileMats[i];
	}

	//remove red tile mats
	for(unsigned i = 0; i < _redTileMats.size(); i++) {
		if(_redTileMats[i] == nullptr) continue;

		delete _redTileMats[i];
	}

	//remove blue tile mats
	for(unsigned i = 0; i < _blueTileMats.size(); i++) {
		if(_blueTileMats[i] == nullptr) continue;

		delete _blueTileMats[i];
	}

	//remove destination tile mats
	delete _redDestinationMat;
	delete _blueDestinationMat;

	//remove red pressure plate mats
	for(unsigned i = 0; i < _redPressurePlateMats.size(); i++) {
		if(_redPressurePlateMats[i] == nullptr) continue;

		delete _redPressurePlateMats[i];
	}

	//remove blue pressure plate mats
	for(unsigned i = 0; i < _bluePressurePlateMats.size(); i++) {
		if(_bluePressurePlateMats[i] == nullptr) continue;

		delete _bluePressurePlateMats[i];
	}

	//remove red activatable tile mats
	for(unsigned i = 0; i < _redActivatableMats.size(); i++) {
		if(_redActivatableMats[i] == nullptr) continue;

		delete _redActivatableMats[i];
	}

	//remove blue activatable tile mats
	for(unsigned i = 0; i < _blueActivatableMats.size(); i++) {
		if(_blueActivatableMats[i] == nullptr) continue;

		delete _blueActivatableMats[i];
	}

	//remove color switch mats
	delete _redColorSwitchMat;
	delete _blueColorSwitchMat;
}
