#ifndef ACTIVATABLETILE_H
#define ACTIVATABLETILE_H

#include "mge/core/GameObject.hpp"
#include "mge\level\SpecialTile.h"
#include "glm.hpp"

class ActivatableTile :public SpecialTile
{
	public:
		ActivatableTile(int pColPos, int pRowPos, int pVectorPos, int pID);
		~ActivatableTile();
		bool CheckPositionOnGrid(int pCol, int pRow);

		void Activate();
		void Reset();

		bool IsActive();
	private:
		int _id;
		int _col; 
		int _row; 
		bool _active;
};

#endif