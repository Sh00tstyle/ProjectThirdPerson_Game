#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "glm.hpp"

#include "mge/core/Renderer.hpp"

#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/GameObject.hpp"

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/materials/LitMaterial.h"
#include "mge/materials/TerrainMaterial.h"

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/LightControlBehaviour.hpp"
#include "mge/behaviours/CameraOrbitBehaviour.h"
#include "mge/behaviours/GridMovementBehavior.hpp"

#include "mge/level/Tile.hpp"
#include "mge/player/Pawn.hpp"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mge/MGEDemo.hpp"

#include "mge/util/tinyxml2.h"

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
MGEDemo::MGEDemo() :AbstractGame(), _hud(0) {
}

void MGEDemo::initialize() {
	//setup the core part
	AbstractGame::initialize();

	//setup the custom part so we can display some text
	std::cout << "Initializing HUD" << std::endl;
	_hud = new DebugHud(_window);
	std::cout << "HUD initialized." << std::endl << std::endl;
}

//build the game _world
void MGEDemo::_initializeScene() {
	//Meshes
	Mesh* planeMesh = Mesh::load(config::MGE_MODEL_PATH + "plane.obj");
	Mesh* cubeMesh = Mesh::load(config::MGE_MODEL_PATH + "cube_flat.obj");
	//Materials
	AbstractMaterial* whiteColorMat = new ColorMaterial(glm::vec3(1, 1, 1));
	AbstractMaterial* blueColorMat = new ColorMaterial(glm::vec3(0, 0, 1)); 
	//Gameobjects
	Camera* camera = new Camera("camera", glm::vec3(0, 20, 15));
	camera->rotate(-45, glm::vec3(1.0, 0, 0)); 
	_world->add(camera);
	_world->setMainCamera(camera);

	GameObject* worldPivot = new GameObject("worldPivot", glm::vec3(0, 0, 0));
	_world->add(worldPivot);

	_generateLevelFromFile(config::MGE_LEVEL_PATH + "TestLevel.xml", planeMesh, cubeMesh ,whiteColorMat, blueColorMat);
}

void MGEDemo::_generateLevelFromFile(std::string filepath, Mesh* tileMesh, Mesh* cubeMesh ,AbstractMaterial* tileMaterial, AbstractMaterial* pawnMaterial) {

	const char * charPath = filepath.c_str(); //converting the input string to a char so tinyxml can read it

	//Reading the xml level file
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError eResult = doc.LoadFile(charPath);
	if(eResult != tinyxml2::XML_SUCCESS) std::cout << "Failed to load xml file" << std::endl;

	tinyxml2::XMLNode* dummyRoot = doc.FirstChild(); //dirty right now, has to change when we are adding new roots to the object

	tinyxml2::XMLNode* playfieldNode = dummyRoot->NextSibling();
	if(playfieldNode == nullptr) std::cout << "Null in root" << std::endl;

	tinyxml2::XMLElement* playfieldElement = playfieldNode->FirstChildElement("Playfield");
	if(playfieldElement == nullptr) std::cout << "Null in element" << std::endl;

	int height, width;
	playfieldElement->QueryIntAttribute("Height", &height);
	playfieldElement->QueryIntAttribute("Width", &width);

	const char* arraymap = playfieldElement->Attribute("Content");
	std::string arrayString(arraymap); //converting char into a string

	std::istringstream iss(arrayString);
	// vector of chars that represent tile values, split arrayString by " "
	std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

	//converting vector of string into 2d array of ints
	for(unsigned i = 0; i < results.size(); i++) {
		int parsedInt = std::stoi(results[i]); //parsing string to an int

		//treating the vector as a 2d array
		int col = i % width;
		int row = i / width;

		float tileSpace = 2.5;

		//only create a tile when it should be visible
		if(parsedInt == 1 || parsedInt == 2) {
			Tile* tile = new Tile("tile" + std::to_string(i), glm::vec3(-(col - width / 2.0f) * tileSpace, 0, (row - height / 2.0f) * tileSpace));
			tile->setMesh(tileMesh);
			tile->setMaterial(tileMaterial);
			tile->scale(glm::vec3(1.0f, 1.0f, 1.0f));
			_world->add(tile);
		}

		if (parsedInt == 2) {
			Pawn* pawn = new Pawn("pawn", glm::vec3(-(col - width / 2.0f) * tileSpace, 1, (row - height / 2.0f) * tileSpace));
			pawn->setMesh(cubeMesh);
			pawn->setMaterial(pawnMaterial); 
			pawn->setBehaviour(new GridMovementBehavior(tileSpace,true, col, row, width, results)); 
			_world->add(pawn); 
		}
	}
	readOutVector(results);
}

void MGEDemo::readOutVector(std::vector<std::string>& pVector)
{
	for (unsigned i = 0; i < pVector.size(); i++)
	{
		std::cout << pVector[i];
	}
}


void MGEDemo::_render() {
	AbstractGame::_render();
	_updateHud();
}

void MGEDemo::_updateHud() {
	std::string debugInfo = "";
	debugInfo += std::string("FPS:") + std::to_string((int)_fps) + "\n";

	//Adding the hierarchy and the parent of everything (the world)
	debugInfo += "\n\n" + std::string("Hierarchy:") + "\n" + _world->getName();

	//getting every children in the world object and printing it
	for(int i = 0; i < _world->getChildCount(); i++) {
		debugInfo += "\n\t" + _world->getChildAt(i)->getName();

		for(int j = 0; j < _world->getChildAt(i)->getChildCount(); j++) {
			debugInfo += "\n\t\t" + _world->getChildAt(i)->getChildAt(j)->getName();

			//could potentially get every children of those gameobjects and print their children too, if there were any but 2 iterations is enough for now
			//solution for more and easier iteration would be recursion
		}
	}

	_hud->setDebugInfo(debugInfo);
	_hud->draw();
}

void MGEDemo::_processEvents() {
	sf::Event event;
	bool exit = false;

	//we must empty the event queue
	while(_window->pollEvent(event)) {
		//give all system event listeners a chance to handle events
		//optionally to be implemented by you for example you could implement a
		//SystemEventDispatcher / SystemEventListener pair which allows Listeners to
		//register with the dispatcher and then do something like:
		//SystemEventDispatcher::dispatchEvent(event);

		switch(event.type) {
			case sf::Event::Closed:
				exit = true;
				break;
			case sf::Event::KeyPressed:
				if(event.key.code == sf::Keyboard::Escape) {
					exit = true;
				}

				//Add own key press events here
				break;
			case sf::Event::Resized:
				//would be better to move this to the renderer
				//this version implements nonconstrained match viewport scaling
				glViewport(0, 0, event.size.width, event.size.height);
				break;

			default:
				break;
		}
	}

	if(exit) {
		_window->close();
	}
}

MGEDemo::~MGEDemo() {
	//dtor
}
