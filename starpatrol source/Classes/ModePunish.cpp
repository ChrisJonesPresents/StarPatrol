#include "ModePunish.h"
#include "GameManager.h"

cocos2d::Scene* ModePunish::createScene(Player *playerfile, MissionNode *stage)
{
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = ModePunish::create();

	layer->Setup(playerfile, stage);
	// add layer as a child to scene
	scene->addChild(layer, 0, 100);

	// return the scene
	return scene;

}

bool ModePunish::init()
{
	if (!Layer::init())
	{
		return false;
	}






	return true;
}

void ModePunish::Save(FILE *file)
{
	char v = 6;
	fwrite((void*)&v, sizeof(char), 1, file);
	for (int i = 0; i < 2; i++)
	{
		fwrite((void*)&m_numbers[i], sizeof(int), 1, file);
	}
}

void ModePunish::Setup(Player *playerfile, MissionNode *stage, bool apply)
{
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_player = playerfile;
	if (apply==true)
	{
		for (int i = 0; i < 2; i++)
		{
			int r = 0;
			int span = stage->m_variables[(i * 3) + 2]-stage->m_variables[(i * 3) + 1];
			if (span>0)
			{
				r = rand() % span;
				r = r + stage->m_variables[(i * 3) + 1];
			}
			else
			{
				r = stage->m_variables[(i * 3) + 1];
			}
			if (stage->m_variables[6] != 0)
			{
				r = r - playerfile->getShip()->getDefence(stage->m_variables[6]- 1);
			}
			if (r > 0)
			{
				switch (stage->m_variables[i * 3])
				{
				case 1:
					//inflict hull damage
					if (r > playerfile->getShip()->getHP())
					{
						r = playerfile->getShip()->getHP();
					}
					playerfile->getShip()->Damage(r);
					break;

				case 2:
					//lose fuel
					if (r > playerfile->getShip()->getFuel())
					{
						r = playerfile->getShip()->getFuel();
					}
					playerfile->getShip()->modFuel(r);
					break;

				case 3:
					//lose money
					if (r > playerfile->getMoney())
					{
						r = playerfile->getMoney();
					}
					playerfile->setMoney(playerfile->getMoney() - r);
					break;


				}
			}
			m_numbers[i] = r;
		}

	}
	ssize_t s;
	std::string fullpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/texts/punish.txt");
	unsigned char *buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
	unsigned long length = s;
	CBReader *cbr = new CBReader(buffer, length);

	for (int i = 0; i < 2; i++)
	{
		if (stage->m_variables[i * 3]>0 && m_numbers[i]>0)
		{
			ProgressReader(cbr, stage->m_variables[i * 3]);
			char buffer[5];
			itoa(m_numbers[i], &buffer[0], 10);
			std::string str = "";
			while (true)
			{
				std::string input = cbr->ReadString();
				if (input.find("#") != std::string::npos)
				{
					break;
				}
				if (input.find("VALUE") != std::string::npos)
				{
					str.append(buffer);
				}
				if (input.find("#") == std::string::npos && input.find("VALUE") == std::string::npos)
				{
					str.append(input);
				}
				str.append(" ");
			}
			Label *label = Label::createWithBMFont("art/font.fnt", str);
			label->setWidth(600);
			label->setPosition(origin.x + visibleSize.width / 2, origin.y +400-(200*i));
			this->addChild(label);
		}

		cbr->setReadposition(0);
	}
	delete cbr;

	//build exit button
	ui::Button *button;
	button = ui::Button::create("art/bluebutton0.png", "art/bluebutton1.png", "art/bluebutton2.png");
	button->setPosition(Vec2(origin.x + (visibleSize.width / 2), origin.y + 64));
	button->setScale(1);
	button->setTitleText("Continue");
	this->addChild(button);
	button->addClickEventListener(CC_CALLBACK_1(ModePunish::ButtonCallback0, this));
}

void ModePunish::ButtonCallback0(Ref *pSender)
{
	//continue
	//check for gameover
	if (m_player->getShip()->getHP() < 0)
	{
		//trigger game over if needbe
		Scene *scene = GameManager::getInstance()->getHandler()->getScene(-2);

		Director::sharedDirector()->replaceScene(scene);
	}
	else
	{
		Scene *scene=GameManager::getInstance()->getHandler()->getScene(0);

		Director::sharedDirector()->replaceScene(scene);
	}
}

void ModePunish::Load(CBReader *reader)
{
	for (int i = 0; i < 2; i++)
	{
		m_numbers[i] = reader->ReadBInt();
	}

}

cocos2d::Scene* ModePunish::createSceneLoad(Player *playerfile, MissionNode *stage, CBReader *reader)
{

	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = ModePunish::create();
	layer->Load(reader);
	layer->Setup(playerfile, stage,false);
	// add layer as a child to scene
	scene->addChild(layer, 0, 100);

	// return the scene
	return scene;


}