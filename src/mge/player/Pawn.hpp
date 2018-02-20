#ifndef PAWN_HPP
#define PAWN_HPP

#include "mge\core\GameObject.hpp"
#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/materials/AbstractMaterial.hpp"
//#include "mge/eventSystem/SystemEventListener.hpp"
#include <SFML/Window/Event.hpp>
#include <vector>
#include "glm.hpp"

#pragma once
//class GridMovementBehaviour; 

class Pawn :
	public GameObject
{
	public:
		Pawn(std::string pName, glm::vec3 pPosition);
		~Pawn();
		
		void ChangeState(AbstractMaterial* pMaterial); 
		bool CheckWalkableColor(AbstractMaterial* pMaterial);
		//virtual void onNotify(sf::Event pEvent); 

	private:

		//GridMovementBehavior* _movement; 
		std::vector<std::string> _levelData; 

};
#endif // !PAWN_HPP

