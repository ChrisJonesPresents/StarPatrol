#ifndef SHOPDEF
#define SHOPDEF

#include "CBReader.h"

class ShopDef
{
public:
	int m_supplies[3][2];
	
	int *m_items[2];
	//first value is category
	//category is 1 for items, 2 for modules
	//second value is index
	int m_numitems;

	int *m_recruits[2];
	int m_numrecruits;

	ShopDef();
	~ShopDef();
	bool PreventDupe(int cat, int value);

	ShopDef(CBReader *reader);
	void Save(FILE *file);
};


#endif