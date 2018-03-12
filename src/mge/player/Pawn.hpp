#ifndef PAWN_HPP
#define PAWN_HPP

#include "mge\core\GameObject.hpp"
#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/materials/AbstractMaterial.hpp"
#include <SFML/Window/Event.hpp>
#include <vector>
#include <string>
#include "glm.hpp"

#pragma once
//class GridMovementBehaviour; 

class Pawn :
	public GameObject
{
	public:
		Pawn(std::string pName, glm::vec3 pPosition);
		~Pawn();
		
		void ChangeState(std::string value); 
		bool CheckWalkableColor(AbstractMaterial* pMaterial);
		//virtual void onNotify(sf::Event pEvent); 

		std::string getPawnColor();
		void setPawnColor(std::string color);

	private:

		std::string _pawnColor;

		//GridMovementBehavior* _movement; 
		std::vector<std::string> _levelData; 

};
#endif // !PAWN_HPP

