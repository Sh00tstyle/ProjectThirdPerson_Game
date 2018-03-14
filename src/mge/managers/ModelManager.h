#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <string>
#include <vector>

class Mesh;
class AbstractMaterial;

class ModelManager {

	public:
		ModelManager(std::string filename);
		~ModelManager();

		static Mesh* GetNormalTileMesh();
		static Mesh* GetDestinationTileMesh();
		static Mesh* GetPressurePlateMesh();
		static Mesh* GetActivatableTileMesh();
		static Mesh* GetColorSwitchMesh();

		static Mesh* GetPlayerMesh();

		static Mesh* GetPlane();

		static AbstractMaterial* GetRedPlayerMat();
		static AbstractMaterial* GetBluePlayerMat();

		static AbstractMaterial* GetRndUncoloredMat();
		static AbstractMaterial* GetRndRedTileMat();
		static AbstractMaterial* GetRndBlueTileMat();

		static AbstractMaterial* GetRedDestinationMat();
		static AbstractMaterial* GetBlueDestinationMat();

		static AbstractMaterial* GetRedPressurePlateMat(int id);
		static AbstractMaterial* GetBluePressurePlateMat(int id);

		static AbstractMaterial* GetRedActivatableTileActiveMat(int id);
		static AbstractMaterial* GetRedActivatableTileInactiveMat(int id);
		static AbstractMaterial* GetBlueActivatableTileActiveMat(int id);
		static AbstractMaterial* GetBlueActivatableTileInactiveMat(int id);

		static AbstractMaterial* GetRedColorSwitchMat();
		static AbstractMaterial* GetBlueColorSwitchMat();

	private:

		//models
		static Mesh* _normalTileMesh;
		static Mesh* _destinationTileMesh;
		static Mesh* _pressurePlateMesh;
		static Mesh* _activatableTileMesh;
		static Mesh* _colorSwitchMesh;

		static Mesh* _playerMesh;

		static Mesh* _plane;

		//textures
		static AbstractMaterial* _redPlayerMat;
		static AbstractMaterial* _bluePlayerMat;

		static std::vector<AbstractMaterial*> _uncoloredTileMats;
		static std::vector<AbstractMaterial*> _redTileMats;
		static std::vector<AbstractMaterial*> _blueTileMats;

		static AbstractMaterial* _redDestinationMat;
		static AbstractMaterial* _blueDestinationMat;

		static std::vector<AbstractMaterial*> _redPressurePlateMats;
		static std::vector<AbstractMaterial*> _bluePressurePlateMats;

		static std::vector<AbstractMaterial*> _redActivatableActiveMats;
		static std::vector<AbstractMaterial*> _redActivatableInactiveMats;
		static std::vector<AbstractMaterial*> _blueActivatableActiveMats;
		static std::vector<AbstractMaterial*> _blueActivatableInactiveMats;

		static AbstractMaterial* _redColorSwitchMat;
		static AbstractMaterial* _blueColorSwitchMat;

		static void _loadMeshesAndMats(std::string filename);
		static void _unloadModels();
};

#endif