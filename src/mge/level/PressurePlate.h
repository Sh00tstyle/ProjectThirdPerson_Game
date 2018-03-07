#include "mge/core/GameObject.hpp"
#include "mge\level\SpecialTile.h"
#include "mge\level\ActivatableTile.h"
#include "glm.hpp"

class PressurePlate :public SpecialTile
{
	public:
		PressurePlate(int pColPos, int pRowPos, int pVectorPos, std::string pActivationColor, int pID, int pIndex);
		~PressurePlate();

		std::string GetActivationColor();
		int GetID();

		void SetTargetTile(ActivatableTile* target);
		ActivatableTile* GetTargetTile();
		bool CheckPositionOnGrid(int pCol, int pRow); 
		void Activate();

		int GetIndex();

	private:
		std::string _activationColor;
		int _index;
		int _id;
		int _col; 
		int _row; 

		ActivatableTile* _targetTile;
};