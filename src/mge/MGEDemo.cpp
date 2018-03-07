#include <iostream>
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

#include "mge/behaviours/PlayfieldFocusBehaviour.h"

#include "mge/managers/SceneManager.h"
#include "mge/managers/ModelManager.h"
#include "mge/managers/InputManager.h"

#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/materials/ScrollingMaterial.hpp"

#include "mge/UI/UiContainer.h"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mge/MGEDemo.hpp"

#include <lua.h>

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
MGEDemo::MGEDemo() :AbstractGame(), _hud(0) {
}

void MGEDemo::initialize() {
	//setup the core part
	AbstractGame::initialize();

	//draws the hud and ui
	std::cout << "Initializing MENU/UI" << std::endl;
	_uiContainer = new UiContainer(_window);
	std::cout << "MENU/UI initialized" << std::endl;

	//loading screen
	_drawLoadingScreen();

	//setup the scene
	_initializeScene();

	//setup the custom part so we can display some text
	std::cout << "Initializing HUD" << std::endl;
	_hud = new DebugHud(_window);
	std::cout << "HUD initialized." << std::endl << std::endl;

	UiContainer::SelectMenu("MAIN");
}

//build the game _world
void MGEDemo::_initializeScene() {
	//orthographic camera settings
	_orthoSize = 9.0f;
	_screenRatio = 16.0f / 9.0f; //default 1920x 1080

	//setting up main camera here, since it should always be the same and does not have to be changed
	Camera* camera = new Camera("camera", glm::vec3(0, 0, 0), glm::ortho(-_orthoSize * _screenRatio, _orthoSize * _screenRatio, -_orthoSize, _orthoSize, 0.1f, 1000.0f)); //orthographic camera
	camera->setBehaviour(new PlayfieldFocusBehaviour(-45.0f, -30.0f, 12.0f, 12.0f, camera)); //focus world origin
	_world->add(camera);
	_world->setMainCamera(camera);

	//background "skybox" plane
	Mesh* planeMesh = Mesh::load(config::MGE_MODEL_PATH + "plane.obj");
	AbstractMaterial* planeMat = new ScrollingMaterial(Texture::load(config::MGE_TEXTURE_PATH + "skybox.png")); //replace this with another material and texture for a scrolling shader
	GameObject* backgroundPlane = new GameObject("bgPlane", glm::vec3(0, 0, 0));
	backgroundPlane->scale(glm::vec3(20, 1, 20)); //scale to screensize
	backgroundPlane->rotate(glm::radians(-30.0f), glm::vec3(0, 1, 0)); //face the camera on the y axis
	backgroundPlane->rotate(glm::radians(45.0f), glm::vec3(1, 0, 0)); //face the camera on the x axis
	backgroundPlane->setMesh(planeMesh);
	backgroundPlane->setMaterial(planeMat);
	_world->add(backgroundPlane);

	//create an instace of model manager to load the items (before creating the SceneManager
	_modelManager = new ModelManager(config::MGE_LEVEL_PATH + "ModelConfig.xml");

	//Make a SceneManager class here instead of a scene
	_sceneManager = new SceneManager(_world); //need an instance to load the levels (!!)
	//_sceneManager->LoadFirstScene(); //loads level 1, diable when the menu is implemented
	
	_renderer->setClearColor(119, 129, 136, 1); //grey background
}

void MGEDemo::_drawLoadingScreen() {
	if(_window->isOpen()) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//uncomment, if we use the debug hud
		/**
		glActiveTexture(GL_TEXTURE0);
		_window->pushGLStates();
		/**/

		_uiContainer->draw();

		_window->display();
	}
}

void MGEDemo::_render() {
	AbstractGame::_render();
	//_updateHud();

	//drawing menu/ui elements on the screen
	_uiContainer->draw();
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
	_window->setKeyRepeatEnabled(false);

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
				SystemEventDispatcher::SendKeyEvent(event);

				//reset current level
				if(event.key.code == sf::Keyboard::R && InputManager::GetGameInput()) {
					SceneManager::ReloadScene();
				}

				//DEBUG

				//load next level
				if(event.key.code == sf::Keyboard::F && InputManager::GetGameInput()) {
					SceneManager::LoadNextScene();
				}

				//reset game to first level
				if (event.key.code == sf::Keyboard::L && InputManager::GetGameInput()) {
					SceneManager::LoadFirstScene();
				}
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

	delete _hud;

	delete _uiContainer;
	delete _modelManager;
}
