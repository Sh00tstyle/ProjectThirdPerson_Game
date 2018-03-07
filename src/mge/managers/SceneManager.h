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

		static void LoadFirstScene();
		static void LoadNextScene();
		static void LoadSceneAtIndex(int index);
		static void ReloadScene();

		static Scene* GetCurrentScene();
		static int GetSceneCount();
		static int GetLevelNumber();

		static int GetLevelTries();

	private:

		static World* _world;

		static int _levelCount;
		static int _levelTries;

		static Scene* _currentScene;
		static std::vector<Scene*> _allScenes;

		static void _loadAllScenes();
		static bool _fileExists(std::string filepath);
};

#endif