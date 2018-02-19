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

		void Activate();

		bool IsActive();
	private:
		int _id;

		bool _active;
};

#endif