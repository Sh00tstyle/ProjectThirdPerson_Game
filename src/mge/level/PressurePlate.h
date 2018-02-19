#include "mge/core/GameObject.hpp"
#include "mge\level\SpecialTile.h"
#include "mge\level\ActivatableTile.h"
#include "glm.hpp"

class PressurePlate :public SpecialTile
{
	public:
		PressurePlate(int pColPos, int pRowPos, int pVectorPos, std::string pActivationColor, int pID);
		~PressurePlate();

		std::string GetActivationColor();
		int GetID();

		void SetTargetTile(ActivatableTile* target);
		ActivatableTile* GetTargetTile();

		void Activate();
	private:
		std::string _activationColor;
		int _id;

		ActivatableTile* _targetTile;
};