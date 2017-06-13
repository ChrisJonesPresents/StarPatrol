#include "ShopLayer.h"
#include "ReturnableLayer.h"

bool ShopLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//build frame
	Sprite *sprite = Sprite::create("art/patternsquareui.png"); 
	sprite->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	this->addChild(sprite);

	//build cancel and purchase buttons
	ui::Button *buttons[4];
	buttons[0] = ui::Button::create("art/shopbuttone0.png", "art/shopbuttone1.png", "art/bluebutton2.png");
	buttons[0]->addClickEventListener(CC_CALLBACK_1(ShopLayer::ButtonCallback0, this));
	buttons[0]->setPosition(Vec2(origin.x + visibleSize.width / 2 - 48, origin.y + visibleSize.height/2 - 92));
	buttons[1] = ui::Button::create("art/shopbuttonf0.png", "art/shopbuttonf1.png", "art/bluebutton2.png");
	buttons[1]->addClickEventListener(CC_CALLBACK_1(ShopLayer::ButtonCallback1, this));
	buttons[1]->setPosition(Vec2(origin.x + visibleSize.width / 2 +48, origin.y + visibleSize.height/2 - 92));
	//build plus and minus buttons
	buttons[2] = ui::Button::create("art/shopminus0.png", "art/shopminus1.png", "art/bluebutton2.png");
	buttons[2]->addClickEventListener(CC_CALLBACK_1(ShopLayer::ButtonCallback3, this));
	buttons[2]->setPosition(Vec2(origin.x + visibleSize.width / 2 - 94, origin.y + visibleSize.height/2 - 60));
	buttons[3] = ui::Button::create("art/shopplus0.png", "art/shopplus1.png", "art/bluebutton2.png");
	buttons[3]->addClickEventListener(CC_CALLBACK_1(ShopLayer::ButtonCallback2, this));
	buttons[3]->setPosition(Vec2(origin.x + visibleSize.width / 2 + 94, origin.y + visibleSize.height/2 - 60));

	for (int i = 0; i < 4; i++)
	{
		this->addChild(buttons[i]);
	}

	return true;
}

void ShopLayer::Setup(int type, int *variable, int max, Layer *layer,int cost)
{
	this->setTouchEnabled(true);
	m_v = type;
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_returnlayer = layer;
	m_max = max;
	m_cost = cost;
	m_variable = variable;
	*m_variable = 10;
	if (*m_variable > max)
	{
		*m_variable = max;
	}
	Label *label = NULL;
	char buffer[6];
	itoa(cost, &buffer[0], 10);
	std::string str = buffer; str.append(" credits per unit");
	//build segbar
	m_bar = Segbar::create();
	m_bar->setPosition(origin.x + visibleSize.width / 2-80, origin.y + visibleSize.height / 2 - 60);
	m_bar->setScale(2.0F);
	switch (type)
	{
	case 0:
		//hull repair
		label = Label::createWithBMFont("art/font.fnt", "How much hull repair should we purchase for "+str);
		m_bar->Setup(0, 2, 20, *m_variable);
		break;
	case 1:
		//fuel
		label = Label::createWithBMFont("art/font.fnt", "How much nuclear fuel should we purchase for "+str);
		m_bar->Setup(2, 2, 20, *m_variable);
		break;
	case 2:
		//ordinance
		label = Label::createWithBMFont("art/font.fnt", "How much starship ordinance should we purchase for " + str);
		m_bar->Setup(1, 2, 20, *m_variable);
		break;
	case 3:
		//crew dismissal
		label = Label::createWithBMFont("art/font.fnt", "How many of this type of crew should we dismiss from the roster?");
		m_bar->Setup(3, 2, 20, *m_variable);
		break;

	case 4:
		//crew hire
		label = Label::createWithBMFont("art/font.fnt", "How many of this type of crew should we hire for "+ str);
		m_bar->Setup(3, 2, 20, * m_variable);
		break;

	case 5:
		//crew dispatch
		label = Label::createWithBMFont("art/font.fnt", "How many of this type of crew should we dispatch on this awayteam? ");
		m_bar->Setup(3, 2, 20, *m_variable);
		break;
	}
	label->setWidth(224);
	label->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 56);
	label->setSystemFontSize(14);
	this->addChild(label);
	_itoa(*m_variable, &buffer[0], 10);
	m_counts[0] = Label::createWithBMFont("art/font.fnt", buffer);
	m_counts[0]->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 32);
	this->addChild(m_counts[0]);
	this->addChild(m_bar);
}

void ShopLayer::ButtonCallback0(Ref *pSender)
{
	//cancel
	*m_variable = 0;
	ReturnableLayer *shop = (ReturnableLayer*)m_returnlayer;
	shop->ReturntoLayer();
	this->getParent()->removeChild(this);
}

void ShopLayer::ButtonCallback1(Ref *pSender)
{
	//okay
	ReturnableLayer *shop = (ReturnableLayer *)m_returnlayer;
	shop->ReturntoLayer();
	this->getParent()->removeChild(this);
}


void ShopLayer::ButtonCallback2(Ref *pSender)
{
	//plus
	if (*m_variable < m_max)
	{
		*m_variable+=1;
		m_bar->setVariable(*m_variable);
		char buffer[4];
		_itoa(*m_variable, &buffer[0], 10);
		m_counts[0]->setString(buffer);
	}
}

void ShopLayer::ButtonCallback3(Ref *pSender)
{
	//minus
	if (*m_variable>0)
	{
		*m_variable-=1;
		m_bar->setVariable(*m_variable);
		char buffer[4];
		_itoa(*m_variable, &buffer[0], 10);
		m_counts[0]->setString(buffer);
	}
}

void ShopLayer::onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event)
{
	Touch *touch = touches[0];
	Rect rect = Rect(m_bar->getPosition().x, m_bar->getPosition().y - 16, 160, 32);
	if (rect.containsPoint(touch->getLocation()))
	{
		float x = touch->getLocation().x - rect.origin.x;
		x = x / rect.size.width;
		x = x * 20;
		x += 1;
		*m_variable = x;
		if (*m_variable > m_max)
		{
			*m_variable = m_max;
		}
		m_bar->setVariable(*m_variable);
		char buffer[4];
		_itoa(*m_variable, &buffer[0], 10);
		m_counts[0]->setString(buffer);
	}
}

void ShopLayer::onTouchesMoved(const std::vector< Touch * > &touches, Event *unused_event)
{
	Touch *touch = touches[0];
	Rect rect = Rect(m_bar->getPosition().x, m_bar->getPosition().y-16, 160, 32);
	if (rect.containsPoint(touch->getLocation()))
	{
		float x = touch->getLocation().x - rect.origin.x;
		x = x / rect.size.width;
		x = x * 20;
		x += 1;
		*m_variable = x;
		if (*m_variable > m_max)
		{
			*m_variable = m_max;
		}
		m_bar->setVariable(*m_variable);
		char buffer[4];
		_itoa(*m_variable, &buffer[0], 10);
		m_counts[0]->setString(buffer);
	}
}

void ShopLayer::onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event)
{

}

