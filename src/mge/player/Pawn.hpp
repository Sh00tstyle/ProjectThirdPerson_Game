#include "mge\core\GameObject.hpp"
#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/materials/AbstractMaterial.hpp"
#include "glm.hpp"

class Pawn :
	public GameObject
{
	public:
		Pawn(std::string pName, glm::vec3 pPosition);
		~Pawn();
		
		void ChangeState(AbstractMaterial* material); 
		//void CheckWalkable(Tile pTile);
		//void 

	private:
		std::vector<std::string> _levelData; 
};

