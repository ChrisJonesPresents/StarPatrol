#include "ModeWarpgate.h"
#include "Mode_Starscape.h"
#include "SolarView.h"
#include "ModeShop.h"
#include "ModeRecruit.h"

cocos2d::Scene* ModeWarpgate::createScene(Player *playerfile)
{
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = ModeWarpgate::create();

	layer->Setup(playerfile);
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;

}

ModeWarpgate::~ModeWarpgate()
{


}

bool ModeWarpgate::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *background = Sprite::create("art/warpgatebackground.png");
	background->setScale(2);
	background->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	this->addChild(background);
	ui::Button *buttons[4];
	buttons[0] = ui::Button::create("art/colonybuttona0.png", "art/colonybuttona1.png", "art/bluebutton2.png");
	buttons[0]->addClickEventListener(CC_CALLBACK_1(ModeWarpgate::ButtonCallback0, this));

	buttons[1] = ui::Button::create("art/colonybuttonb0.png", "art/colonybuttonb1.png", "art/bluebutton2.png");
	buttons[1]->addClickEventListener(CC_CALLBACK_1(ModeWarpgate::ButtonCallback1, this));

	buttons[2] = ui::Button::create("art/colonybuttonc0.png", "art/colonybuttonc1.png", "art/bluebutton2.png");
	buttons[2]->addClickEventListener(CC_CALLBACK_1(ModeWarpgate::ButtonCallback2, this));

	buttons[3] = ui::Button::create("art/colonybuttond0.png", "art/colonybuttond1.png", "art/bluebutton2.png");
	buttons[3]->addClickEventListener(CC_CALLBACK_1(ModeWarpgate::ButtonCallback3, this));
	for (int i = 0; i < 4; i++)
	{
		buttons[i]->setPosition(Vec2(origin.x + 32 + (64 * i), origin.y + 16));
	//	buttons[i]->setScale(2);
		this->addChild(buttons[i]);
	}


	Sprite *frame = Sprite::create("art/frame.png");
	this->addChild(frame);
	frame->setScale(2);
	frame->setPosition(origin.x + visibleSize.width - 128, origin.y + 128);

	Label *label = Label::createWithBMFont("art/font.fnt", 
		"The Warpgate constructed by the pioneer team uses the space warping power of a black hole to create an Einstein Rosen bridge to Star Patrol Command over a thousand lightyears away.");
	label->setPosition(frame->getPosition());
	label->setAnchorPoint(Vec2(0.5F, 0.5F));
	this->addChild(label);
	label->setSystemFontSize(20);
	label->setWidth(250);
	return true;
}

void ModeWarpgate::Setup(Player *playerfile)
{
	m_player = playerfile;
}

void ModeWarpgate::ButtonCallback0(Ref *pSender)
{
	//button menu
}

void ModeWarpgate::ButtonCallback1(Ref *pSender)
{
	//button shop
	Scene *scene = ModeShop::createSceneWarpgate(m_player);
	Director::sharedDirector()->replaceScene(scene);
}

void ModeWarpgate::ButtonCallback2(Ref *pSender)
{

	//button recruit
	Scene *scene = ModeRecruit::createSceneWarpgate(m_player);
	Director::sharedDirector()->replaceScene(scene);
}

void ModeWarpgate::ButtonCallback3(Ref *pSender)
{
	//button close view
	Scene *scene = Mode_Starscape::createScene(true);

	//attach solarviewer
	Player *p = GameManager::getInstance()->getPlayer();

	//is this the star the player is currently orbiting?
	//later, zoom in on star contents when this happens


	Director::sharedDirector()->replaceScene(scene);
//	SolarView *view = SolarView::create();

//	view->Setup(layer, GameManager::getInstance()->getStars()[(int)p->getPos0().y][(int)p->getPos0().x]);

	//this->getParent()->addChild(view);	
	//scene->addChild(view);
}