#include "mge\level\SpecialTile.h"
#include "glm.hpp"

class DestinationTile :public SpecialTile
{
	public:
		DestinationTile(int pColPos, int pRowPos, int pVectorPos, std::string pNeededColor);
		~DestinationTile();

		std::string GetNeededColor();
	private:
		std::string _neededColor;
};