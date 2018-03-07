#ifndef ACTIVATABLETILE_H
#define ACTIVATABLETILE_H

#include "mge/core/GameObject.hpp"
#include "mge\level\SpecialTile.h"
#include "glm.hpp"

class Scene;

class ActivatableTile :public SpecialTile
{
	public:
		ActivatableTile(int pColPos, int pRowPos, int pVectorPos, int pID, Scene* pScene);
		~ActivatableTile();
		bool CheckPositionOnGrid(int pCol, int pRow);

		void Activate();
		void Reset();

		bool IsActive();

		void SetColor(std::string color);

		void SetIndex(int index);
		int GetIndex();

	private:
		Scene * _scene;

		std::string _color;

		int _index;
		int _id;
		int _col; 
		int _row; 
		bool _active;
};

#endif