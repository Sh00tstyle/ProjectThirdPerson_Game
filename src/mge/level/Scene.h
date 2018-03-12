#include <string>
#include <vector>
#include <memory>
#include <glm.hpp>

class World;
class Mesh;
class Pawn;
class AbstractMaterial;
class GameObject;
class PressurePlate;
class ActivatableTile;
class SpawnTile;
class DestinationTile;

class Scene {

	public:
		Scene(std::string filepath, World* pWorld);
		~Scene();
		
		void ConstructScene();
		void RemoveScene();

		int GetLevelWidth();
		int GetLevelHeight();

		int GetPlayfieldDataCount();
		std::string GetPlayfieldValue(int vectorIndex); //for treating it like a vector
		std::string GetPlayfieldValue(int colIndex, int rowIndex); //for treating it like a 2d array
		glm::vec3 GetTileWorldPos(int colIndex, int rowIndex, float pTileSize);
		void SetPlayfieldColor(int vectorIndex, std::string value); //for treating it like a vector
		void SetPlayfieldColor(int colIndex, int rowIndex, std::string value); //for treating it like a 2d array
		void SetPawnColor( std::string value); 
		std::string GetPawnColor();
		std::string GetDestinationColor();
		std::string GetStartTileColor();
		PressurePlate* GetPressurePlate(int colIndex, int rowIndex); 
		ActivatableTile* GetActivatableTile(int colIndex, int rowIndex); 
		GameObject* GetTileObject(int col, int row);

	private:
		World* _world;

		//tiles
		std::vector<GameObject*> _tileObjects;

		//player
		Pawn* _pawn;

		//playfield
		int _levelWidth;
		int _levelHeight;

		std::vector<std::string> _playfieldData;
		std::vector<std::string> _unmodifedPlayfieldData; //backup to reset the level later

		//hint properties
		std::vector<std::vector<std::string>> _hintData;
		int _triesPerHint;

		//scene objetcs
		std::vector<GameObject*> _sceneObjects;
		std::vector<AbstractMaterial*> _sceneObjectMats;
		std::vector<Mesh*> _sceneObjectMeshes;

		//pressure plates
		std::vector<PressurePlate*> _pressurePlates;

		//activatable tiles
		std::vector<ActivatableTile*> _activatableTiles;

		//spawn tile
		SpawnTile* _spawnTile;

		//destination tile
		DestinationTile* _destinationTile;

		void _loadSceneFromFile(std::string filepath);
		void _setTileMaterial(GameObject* newTile, std::string tileProperty, float xPos, float zPos, int i);
		void _setTileMesh(GameObject* newTile, std::string tileProperty);

		void _destructScene();
};