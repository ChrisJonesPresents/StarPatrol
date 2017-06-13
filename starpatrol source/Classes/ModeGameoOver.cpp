#include "ModeGameOver.h"
#include "GameManager.h"
#include "ModeMenu.h"

bool ModeGameOver::init()
{
	if (!Layer::init())
	{
		return false;
	}


	return true;
}

cocos2d::Scene* ModeGameOver::createScene(MissionNode *stage)
{
	ModeGameOver *mode = ModeGameOver::create();
	Scene *scene = Scene::create();
	scene->addChild(mode);
	if (stage != NULL)
	{
		mode->Setup(stage->m_variables[0]);
	}
	else
	{
		if (GameManager::getInstance()->getPlayer()->getShip()->getHP() <= 0)
		{
			mode->Setup(0);
		}
		else
		{
			mode->Setup(1);
		}
	}


	GameManager::getInstance()->GameOver();
	return scene;

}


void ModeGameOver::Setup(int index)
{
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	ssize_t s;
	unsigned long length;
	std::string fullpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/texts/gameovers.txt");
	unsigned char *buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);

	while (s == 0)
	{
		buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
	}
	length = s;

	CBReader *cbr = new CBReader(buffer, length);

	//read text
	ProgressReader(cbr, index);
	std::string str = "";
	while (true)
	{
		std::string input = cbr->ReadString();

		if (input.find("#") != std::string::npos)
		{
			break;
		}
		else
		{
			if (input.find("SHIP") != std::string::npos)
			{
				str.append(GameManager::getInstance()->getPlayer()->getShip()->getName());
		
			}
			if (input.find("CAPTAIN") != std::string::npos)
			{
				Player *player=GameManager::getInstance()->getPlayer();
				for (int i = 0; i < player->getNumOfficers(); i++)
				{
					if (player->getOfficer(i)!= NULL)
					{
						str.append(player->getOfficer(i)->getName());
					}
				}
			}
			if (input.find("SHIP") == std::string::npos && input.find("CAPTAIN") == std::string::npos)
			{
				str.append(input);
			}
			str.append(" ");
		}
	}
	delete cbr;

	//draw gameover text
	Label *label = Label::createWithBMFont("art/font.fnt",str);
	label->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	this->addChild(label);
	label->setWidth(512);

	//draw button to return to menu
	ui::Button *button;
	button= ui::Button::create("art/bluebutton0.png", "art/bluebutton1.png", "art/bluebutton2.png");
	button->addClickEventListener(CC_CALLBACK_1(ModeGameOver::ButtonCallback, this));
	button->setTitleText("BACK");
	this->addChild(button);
	button->setPosition(Vec2(origin.x + (visibleSize.width / 2), origin.y + 100));


}

void ModeGameOver::ButtonCallback(cocos2d::Ref* pSender)
{
	Scene *scene = ModeMenu::createScene(0);
	Director::sharedDirector()->replaceScene(scene);
}