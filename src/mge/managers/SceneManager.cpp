#include "mge/managers/SceneManager.h"
#include "mge/level/Scene.h"
#include "mge/core/World.hpp"
#include "mge/config.hpp"
#include "mge/UI/UiContainer.h"
#include "mge/MGEDemo.hpp"
#include <fstream>

World* SceneManager::_world; 
Scene* SceneManager::_currentScene; 
int SceneManager::_levelCount = int(); 
int SceneManager::_levelTries = int();
std::vector<Scene*> SceneManager::_allScenes = std::vector<Scene*>(); 

SceneManager::SceneManager(World* pWorld) {
	_world = pWorld;

	_loadAllScenes();
}

SceneManager::~SceneManager() {
	//cleanup
	for(unsigned i = 0; i < _allScenes.size(); i++) {
		delete _allScenes[i];
	}
}

void SceneManager::LoadFirstScene() {
	if (_currentScene != nullptr) _currentScene->RemoveScene();

	_levelCount = 0;
	_levelTries = 1;

	UiContainer::ResetHints();

	//no first level (empty array)
	if((int)_allScenes.size() == 0) {
		std::cout << "Failed to load first scene, no scenes found" << std::endl;
		return;
	}

	_currentScene = _allScenes[0];
	if(_currentScene != nullptr) _currentScene->ConstructScene(false);
}

void SceneManager::LoadNextScene() {
	_levelCount++;
	_levelTries = 1;

	UiContainer::ResetHints();

	//no more levels to load
	if(_levelCount >= (int)_allScenes.size()) {
		std::cout << "No more levels to load" << std::endl;
		return;
	}

	if(_currentScene != nullptr) _currentScene->RemoveScene();

	_currentScene = _allScenes[_levelCount];
	if(_currentScene != nullptr) _currentScene->ConstructScene(false);
}

void SceneManager::LoadSceneAtIndex(int index) {
	if(_currentScene != nullptr) _currentScene->RemoveScene();

	_levelCount = index;
	_levelTries = 1;

	UiContainer::ResetHints();

	//out of array range
	if(_levelCount >= (int)_allScenes.size()) {
		std::cout << "Index out of scene vector range" << std::endl;
		return;
	}

	if(_currentScene != nullptr) _currentScene->RemoveScene();

	_currentScene = _allScenes[_levelCount];
	if(_currentScene != nullptr) _currentScene->ConstructScene(false);
}

void SceneManager::ReloadScene(bool hint) {
	if(_currentScene != nullptr) {
		_levelTries++;

		_currentScene->RemoveScene();
		_currentScene->ConstructScene(hint);
	}
}

Scene* SceneManager::GetCurrentScene() {
	return _currentScene;
}

int SceneManager::GetSceneCount() {
	return (int)_allScenes.size();
}

int SceneManager::GetLevelNumber() {
	return _levelCount + 1;
}

int SceneManager::GetLevelTries() {
	return _levelTries;
}

void SceneManager::SetLevelTries(int tries) {
	_levelTries = tries;
}

//blocking call right now, maybe spawn a thread for loading a mesh in order to accelerate loading (probably not needed)
void SceneManager::_loadAllScenes() {
	int levelIndex = 1;
	std::string filepath = config::MGE_LEVEL_PATH + "Level_" + std::to_string(levelIndex) + ".xml";

	int percentageCount = 0;

	while(_fileExists(filepath)) { //only load one level for now
		Scene* newScene = new Scene(filepath, _world);
		MGEDemo::UpdateLoadingScreen(64 + percentageCount);
		_allScenes.push_back(newScene);

		levelIndex++;
		filepath = config::MGE_LEVEL_PATH + "Level_" + std::to_string(levelIndex) + ".xml";

		percentageCount += 4;
	}
}

bool SceneManager::_fileExists(std::string filepath) {
	std::ifstream ifile(filepath.c_str());
	return (bool)ifile;
}
