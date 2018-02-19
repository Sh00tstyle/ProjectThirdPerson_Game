#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "glm.hpp"

//delete as soon as scene class is done
#include <sstream>
#include "mge/util/tinyxml2.h"

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
#include "mge/behaviours/PlayfieldFocusBehaviour.h"

#include "mge/level/Scene.h"

#include "mge/player/Pawn.hpp"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mge/tileProp.hpp"
#include "mge/MGEDemo.hpp"

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
	//orthographic camera settings
	_orthoSize = 12.0f;
	_screenRatio = 4.0f / 3.0f; //default 800x 600

	//setting up main camera here, since it should always be the same and does not have to be changed
	Camera* camera = new Camera("camera", glm::vec3(0, 0, 0), glm::ortho(-_orthoSize * _screenRatio, _orthoSize * _screenRatio, -_orthoSize, _orthoSize, 0.1f, 1000.0f)); //orthographic camera
	camera->setBehaviour(new PlayfieldFocusBehaviour(-40.0f, -60.0f, 12.0f, 12.0f, camera)); //focus world origin
	_world->add(camera);
	_world->setMainCamera(camera);

	//Make a SceneManager class here instead of a scene
	std::unique_ptr<Scene> levelScene(new Scene(config::MGE_LEVEL_PATH + "Level_3.xml", _world)); //smart pointer
	levelScene->ConstructScene();

	_renderer->setClearColor(119, 129, 136, 1); //grey background
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

				//readjust the projection matrix when resizing the screen
				_screenRatio = (float)event.size.width / (float)event.size.height;
				_world->getMainCamera()->setProjection(glm::ortho(-_orthoSize * _screenRatio, _orthoSize * _screenRatio, -_orthoSize, _orthoSize, 0.1f, 1000.0f));
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
