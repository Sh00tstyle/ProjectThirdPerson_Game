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

#include "mge/util/tinyxml2.h"
#include "mge/tileProp.hpp"
#include "mge/config.hpp"

Scene::Scene(std::string pFilepath, World* pWorld) {
	_world = pWorld;
	_loadSceneFromFile(pFilepath);
}


Scene::~Scene() {
	//cleanup
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

		//Create Tile
		GameObject* newTile = new GameObject("tile" + std::to_string(i), glm::vec3(xPos, 0, zPos));
		newTile->setMesh(_tileMesh);

		int rnd = 0;

		if(tileProperty == tileProp::Uncolored) {
			rnd = rand() % (_uncoloredTileMats.size());
			newTile->setMaterial(_uncoloredTileMats[rnd]);
		} else if(tileProperty == tileProp::PlayerSpawn) {
			//setting up the player
			_pawn = new Pawn("pawn", glm::vec3(xPos, 1, zPos));
			_pawn->setMesh(_playerMesh);

			//coloring tile and player
			if(_spawnTile->GetStartingColor() == tileProp::RedTile) {
				_pawn->setMaterial(_redPlayerMat);

				rnd = rand() % (_redTileMats.size());
				newTile->setMaterial(_redTileMats[rnd]);
			} else {
				_pawn->setMaterial(_bluePlayerMat);

				rnd = rand() % (_blueTileMats.size());
				newTile->setMaterial(_blueTileMats[rnd]);
			}

			_pawn->setBehaviour(new GridMovementBehavior(tileSize, true, col, row, _levelWidth, _playfieldData));
			_world->add(_pawn);
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
					if(myPlate->GetActivationColor() == tileProp::RedTile) newTile->setMaterial(_redPressurePlateMats[0]);
					else newTile->setMaterial(_bluePressurePlateMats[0]); //error
				}
			}
		} else if(tileProperty == tileProp::ActivatableTile) {
			for(unsigned j = 0; j < _pressurePlates.size(); j++) {
				PressurePlate* myPlate = _pressurePlates[j];
				ActivatableTile* myTile = myPlate->GetTargetTile();

				if(myTile->GetVectorPos() == i) {
					if(myPlate->GetActivationColor() == tileProp::RedTile) newTile->setMaterial(_redActivatableMats[0]);
					else newTile->setMaterial(_blueActivatableMats[0]); //error
				}
			}
		} else if(tileProperty == tileProp::RedColorSwitch) {
			newTile->setMaterial(_redColorSwitchMat);
		} else if(tileProperty == tileProp::BlueColorSwitch) {
			newTile->setMaterial(_blueColorSwitchMat);
		}

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

void Scene::ResetScene() {
	//copying the inital array back to our tracked playfield
	_playfieldData = _unmodifedPlayfieldData;
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

		//model
		readChar = listElement->Attribute("Model");
		std::string modelString(readChar);

		//texture
		readChar = listElement->Attribute("Texture");
		std::string textureString(readChar);

		//create mesh and material for the gameobjects
		Mesh* objMesh = Mesh::load(config::MGE_MODEL_PATH + modelString);
		AbstractMaterial* objMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + textureString));
		GameObject* newGameObject = new GameObject(nameString, glm::vec3(-xPos, yPos, zPos)); //invert x pos

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

		std::cout << "Pressure Plate Vec" + std::to_string(colPos + rowPos * _levelWidth) << std::endl;

		PressurePlate* newPlate = new PressurePlate(colPos, rowPos, colPos + rowPos * _levelWidth, colorString, id);
		_pressurePlates.push_back(newPlate);

		listElement = listElement->NextSiblingElement("PressurePlate");
	}

	//activatable tiles
	element = root->FirstChildElement("ActivatableTiles"); //gp to the next element
	if(element == nullptr) std::cout << "Null in ActivatableTiles" << std::endl;

	listElement = element->FirstChildElement("ActivatableTile");

	while(listElement != nullptr) {
		int colPos;
		int rowPos;
		int id;

		listElement->QueryIntAttribute("ColPos", &colPos);
		listElement->QueryIntAttribute("RowPos", &rowPos);
		listElement->QueryIntAttribute("ID", &id);

		std::cout << "Activatable Tile Vec" + std::to_string(colPos + rowPos * _levelWidth) << std::endl;

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

	std::cout << "Read Scene from XML" << std::endl;
}