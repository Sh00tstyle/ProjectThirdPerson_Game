#include "mge/core/GameObject.hpp"
#include "mge/materials/AbstractMaterial.hpp"
#include "glm.hpp"

class Tile :public GameObject
{
	public:
		Tile(std::string pName, glm::vec3 pPosition);
		~Tile(); 

		void SetColor(AbstractMaterial * pMaterial); 

	private:

};