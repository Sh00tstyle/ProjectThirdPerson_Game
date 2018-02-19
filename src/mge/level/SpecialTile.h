#ifndef SPECIALTILE_H
#define SPECIALTILE_H

class SpecialTile
{
	public:
		SpecialTile(int pColPos, int pRowPos, int pVectorPos);
		~SpecialTile();

		int GetColPos();
		int GetRowPos();
		int GetVectorPos();

	private:
		int _colPos;
		int _rowPos;
		int _vectorPos;
};

#endif