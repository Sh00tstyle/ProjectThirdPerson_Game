#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <memory>
#include <string>
#include "mge/core/World.hpp"

class Scene;

class SceneManager
{
	public:
		SceneManager(World* pWorld);
		~SceneManager();

		void LoadFirstScene();
		void LoadNextScene();
		void LoadSceneAtIndex(int index);
		void ReloadScene();

		Scene* GetCurrentScene();
		int GetSceneCount();
	private:

		World* _world;

		int _levelCount;

		Scene* _currentScene;
		std::vector<Scene*> _allScenes;

		void _loadAllScenes();
		bool _fileExists(std::string filepath);
};

#endif