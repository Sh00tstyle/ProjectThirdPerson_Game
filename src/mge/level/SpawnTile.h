#include "mge\level\SpecialTile.h"
#include "glm.hpp"

class SpawnTile :public SpecialTile
{
	public:
		SpawnTile(int pColPos, int pRowPos, int pVectorPos, std::string pStartingColor);
		~SpawnTile();

		std::string GetStartingColor();
	private:
		std::string _startingColor;
};