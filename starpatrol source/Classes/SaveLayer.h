#ifndef SAVELAYER
#define SAVELAYER

#include "ReturnableLayer.h"

class SaveLayer : public ReturnableLayer
{
public:

	virtual void Save(FILE *file){};

};


#endif