#include "mge/managers/SceneManager.h"
#include "mge/level/Scene.h"
#include "mge/config.hpp"
#include <fstream>

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
	_levelCount = 0;

	//no first level (empty array)
	if((int)_allScenes.size() == 0) {
		std::cout << "Failed to load first scene, no scenes found" << std::endl;
		return;
	}

	_currentScene = _allScenes[0];
	if(_currentScene != nullptr) _currentScene->ConstructScene();
}

void SceneManager::LoadNextScene() {
	_levelCount++;

	//no more levels to load
	if(_levelCount >= (int)_allScenes.size()) {
		std::cout << "No more levels to load" << std::endl;
		return;
	}

	if(_currentScene != nullptr) _currentScene->RemoveScene();

	_currentScene = _allScenes[_levelCount];
	if(_currentScene != nullptr) _currentScene->ConstructScene();
}

void SceneManager::LoadSceneAtIndex(int index) {
	_levelCount = index;

	//out of array range
	if(_levelCount >= (int)_allScenes.size()) {
		std::cout << "Index out of scene vector range" << std::endl;
		return;
	}

	if(_currentScene != nullptr) _currentScene->RemoveScene();

	_currentScene = _allScenes[_levelCount];
	if(_currentScene != nullptr) _currentScene->ConstructScene();
}

void SceneManager::ReloadScene() {
	if(_currentScene != nullptr) {
		_currentScene->RemoveScene();
		_currentScene->ConstructScene();
	}
}

Scene* SceneManager::GetCurrentScene() {
	return _currentScene;
}

int SceneManager::GetSceneCount() {
	return (int)_allScenes.size();
}

void SceneManager::_loadAllScenes() {
	int levelIndex = 1;
	std::string filepath = config::MGE_LEVEL_PATH + "Level_" + std::to_string(levelIndex) + ".xml";

	while(_fileExists(filepath)) {
		Scene* newScene = new Scene(filepath, _world);
		_allScenes.push_back(newScene);

		levelIndex++;
		filepath = config::MGE_LEVEL_PATH + "Level_" + std::to_string(levelIndex) + ".xml";
	}
}

bool SceneManager::_fileExists(std::string filepath) {
	std::ifstream ifile(filepath.c_str());
	return (bool)ifile;
}
