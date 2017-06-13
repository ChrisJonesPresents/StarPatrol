#include "ModeBriefing.h"
#include "CBReader.h"
#include "GameManager.h"

bool ModeBriefing::init()
{
	if (!Layer::init())
	{
		return false;
	}

	return true;
}

std::string BuildString(CBReader *reader, std::string tag)
{
	std::string str = "";
	while (true)
	{
		std::string input = reader->ReadString();
		if (input.find("#") != std::string::npos)
		{
			break;
		}
		else
		{
			if (input.find("/N") != std::string::npos)
			{
				str.append("/n");
			}
			else
			{
				if (input.find("TAG") != std::string::npos)
				{
					str.append(tag + " ");
				}
				else
				{
					str.append(input + " ");
				}
			}
		}

	}
	return str;
}

void ModeBriefing::Setup(MissionNode *stage, std::string tag)
{
	m_stage = stage;
	m_tag = tag;
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//check if stage is choice or briefing
	if (m_stage->m_type == MST_CHOICE)
	{
		ssize_t s;
		unsigned long length;
		std::string fullpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/missions/decisions.txt");
		unsigned char *buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
		while (s < 1)
		{
			buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
		}
		length = s;

		CBReader *cbr = new CBReader(buffer, length);
		ProgressReader(cbr, m_stage->m_variables[0]);
		Label *label = Label::createWithBMFont("art/font.fnt", BuildString(cbr,m_tag));
		label->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
		label->setWidth(512);
		this->addChild(label);
		cocos2d::ui::Button *buttons[2];
		delete cbr;
	

		fullpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/missions/buttontexts.txt");
		buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
		length = s;
		cbr = new CBReader(buffer, length);
		//attach choices
		for (int i = 0; i < 2; i++)
		{
			buttons[i] = ui::Button::create("art/bluebutton0.png", "art/bluebutton1.png", "art/bluebutton2.png");
			buttons[i]->setPosition(Vec2(origin.x + (visibleSize.width / 2) - 64 + (128 * i), origin.y + 64));
			buttons[i]->setScale(1);
		
		
			ProgressReader(cbr, m_stage->m_variables[2+i]);
			buttons[i]->setTitleText(BuildString(cbr, m_tag));
			this->addChild(buttons[i]);
			cbr->setReadposition(0);
		}
		buttons[0]->addClickEventListener(CC_CALLBACK_1(ModeBriefing::ButtonCallback0, this));
		buttons[1]->addClickEventListener(CC_CALLBACK_1(ModeBriefing::ButtonCallback1, this));

		bool b = false;
		//conditional
		if (m_stage->m_variables[4] > 0)
		{
			switch (m_stage->m_variables[4])
			{
			case 1:
				//module
				//check for module
				for (int i = 0; i < GameManager::getInstance()->getPlayer()->getShip()->getNumModules(); i++)
				{
					if (GameManager::getInstance()->getPlayer()->getShip()->getModule(i) != NULL)
					{
						if (GameManager::getInstance()->getPlayer()->getShip()->getModule(i)->getUID() == m_stage->m_variables[5])
						{
							b = true;
							break;
						}
					}
				}
				if (b == false)
				{
					buttons[0]->setBright(false);
					buttons[0]->setEnabled(false);
				}
				break;

			case 2:
				//item
				for (int i = 0; i < 3; i++)
				{
					if (GameManager::getInstance()->getPlayer()->getItem(i) != NULL)
					{
						if (GameManager::getInstance()->getPlayer()->getItem(i)->m_uid == m_stage->m_variables[5])
						{
							b = true;
							break;
						}
					}
				}
				if (b == false)
				{
					buttons[0]->setBright(false);
					buttons[0]->setEnabled(false);
				}
				//check items
				break;
			case 3:
				//officer
				//check for officer
				for (int i = 0; i < GameManager::getInstance()->getPlayer()->getNumOfficers(); i++)
				{
					if (GameManager::getInstance()->getPlayer()->getOfficer(i) != NULL)
					{
						if (GameManager::getInstance()->getPlayer()->getOfficer(i)->getDefinition()->getUID() == m_stage->m_variables[5])
						{
							b = true;
							break;
						}
					}
				}
				if (b == false)
				{
					buttons[0]->setBright(false);
					buttons[0]->setEnabled(false);
				}
				break;
			case 4:
				//money
				if (GameManager::getInstance()->getPlayer()->getMoney() < m_stage->m_variables[5])
				{
					buttons[0]->setBright(false);
					buttons[0]->setEnabled(false);
				}
				//check for money
				break;


			}
		}
		delete cbr;
	}
	else
	{
		ssize_t s;
		unsigned long length;
		std::string fullpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/missions/briefings.txt");
		char *buffer = (char*)cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
		while (s < 1)
		{
			buffer = (char*)cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
		}	
		length = s;
		CBReader *cbr = new CBReader(buffer, length);
		ProgressReader(cbr, m_stage->m_variables[0]);
		Label *label = Label::createWithBMFont("art/font.fnt", BuildString(cbr,m_tag));
		label->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
		this->addChild(label);
		label->setWidth(512);
		//attach continue button
		cocos2d::ui::Button *button;
		button = ui::Button::create("art/bluebutton0.png", "art/bluebutton1.png", "art/bluebutton2.png");
		button->addClickEventListener(CC_CALLBACK_1(ModeBriefing::ButtonCallback0, this));
		button->setPosition(Vec2(origin.x + (visibleSize.width / 2), origin.y + 64));
		button->setScale(2);
		button->setTitleText("Ok");
		this->addChild(button);

	}
	
}

void ModeBriefing::ButtonCallback0(Ref *pSender)
{
	Scene *scene=GameManager::getInstance()->getHandler()->getScene(0);
	Director::sharedDirector()->replaceScene(scene);
	SaveLayer *layer = (SaveLayer*)scene->getChildByTag(100);
	if (layer != NULL)
	{
		GameManager::getInstance()->getHandler()->setLayer(layer);
	}
}

void ModeBriefing::ButtonCallback1(Ref *pSender)
{
	Scene *scene = GameManager::getInstance()->getHandler()->getScene(1);
	Director::sharedDirector()->replaceScene(scene);

}

void ModeBriefing::ButtonCallback2(Ref *pSender)
{
	Scene *scene = GameManager::getInstance()->getHandler()->getScene(m_stage->m_variables[5]);
	Director::sharedDirector()->replaceScene(scene);

}

void ModeBriefing::Save(FILE *file)
{
	char v = 3;
	fwrite((void*)&v, sizeof(char), 1, file);
}