#ifndef MODEBRIEFING
#define MODEBRIEFING

#include "Mission2.h"
#include <ui\UIButton.h>
#include "SaveLayer.h"

class ModeBriefing : public SaveLayer
{
	MissionNode *m_stage;
	std::string m_tag;

public:


	virtual bool init();

	void Setup(MissionNode *stage,std::string tag);

	CREATE_FUNC(ModeBriefing);

	void ButtonCallback0(Ref *pSender);

	void ButtonCallback1(Ref *pSender);
	void ButtonCallback2(Ref *pSender);

	void Save(FILE *file);
	void ReturntoLayer(){};
};



#endif