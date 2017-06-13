#include "OfficerView.h"
#include "CrewPanel.h"
#include "RenamePanel.h"

bool OfficerView::init()
{
	if (!Layer::init())
	{
		return false;
	}
	m_selection = 0;

	this->setTouchEnabled(true);

	return true;
}

void OfficerView::Setup(Officer *officer, Layer *hostlayer,Label *description)
{
	m_officer = officer;
	m_layer = hostlayer;
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_description = description;
	//build frame

	m_panel= Sprite::create("art/officerframeexp.png");
	m_panel->setScale(2);
	m_panel->setPosition(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height / 2)+64);
	this->addChild(m_panel);
	m_panel->setZOrder(1);
	//build portrait
	Sprite *spr = Sprite::create("art/officers.png", Rect(m_officer->getDefinition()->getPortrait().x * 128, m_officer->getDefinition()->getPortrait().y * 128, 128, 128));
	spr->setPosition(m_panel->getPosition() + Vec2(0, 128));
	spr->setScale(2);

	this->addChild(spr);

	for (int i = 0; i < 7; i++)
	{
		m_labelframes[i] = Sprite::create("art/backbox2.png");
		m_labelframes[i]->setPosition(Vec2(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height / 2) + 48-(32*i)));
		this->addChild(m_labelframes[i]);

		m_labels[i] = Label::createWithBMFont("art/font.fnt", "null");
		m_labels[i]->setPosition(Vec2(64, 16)); 
		m_labelframes[i]->setScaleX(2.0F);
	//	m_labels[i]->setAnchorPoint(Vec2(0.5F, 1));
		//m_labels[i]->setPosition(Vec2(64, 120- (17 * i)));
		m_labels[i]->setSystemFontSize(16);
		m_labelframes[i]->setZOrder(2);
		m_labels[i]->setZOrder(3);

		m_labelframes[i]->addChild(m_labels[i]);
	}
	
	//build nametag
	m_labels[0]->setString(m_officer->getName());
	//build level
	char buffer[4];
	itoa(m_officer->getLevel(), &buffer[0], 10);
	std::string str = "level "; str.append(buffer);
	m_labels[1]->setString(str);
	//build experience
	itoa(m_officer->getExperience(), &buffer[0], 10);
	str = "EXP:"; str.append(buffer); str.append("/2");
	int v = 2 + m_officer->getLevel();
	v = v*v;
	itoa(v, &buffer[0], 10);
	str.append(buffer);
	m_labels[2]->setString(str);
	//build abilities 1-4
	for (int i = 3; i < 7; i++)
	{
		if (m_officer->getAbility(i-3)!=NULL)
		{
			m_labels[i]->setString(m_officer->getAbility(i - 3)->getName());
		}
		else
		{
			m_labels[i]->setString("");
		}
	}




	if (m_officer->CanLevel())
	{
		//build subpanel
		BuildSubPanel();

	}
	else
	{
		m_subpanel = NULL;
	}

	//build left panel for stats
	m_frame = Sprite::create("art/frame.png");
	m_frame->setPosition(m_panel->getPositionX() + 256, m_panel->getPositionY()-128);
	m_frame->setScaleX(2);
	m_frame->setScaleY(2);
	this->addChild(m_frame);

	//build escape button
	ui::Button *button = ui::Button::create("art/bluebutton0.png", "art/bluebutton1.png", "art/bluebutton2.png");
	button->setTitleText("Escape");
	button->setPosition(Vec2(m_panel->getPositionX() + 320, m_panel->getPositionY()-230));
	button->setTitleFontSize(32);
	button->addClickEventListener(CC_CALLBACK_1(OfficerView::ButtonCallback0, this));
	this->addChild(button);
	//build rename button
	button = ui::Button::create("art/bluebutton0.png", "art/bluebutton1.png", "art/bluebutton2.png");
	button->setTitleText("Rename");
	button->setPosition(Vec2(m_panel->getPositionX() + 194, m_panel->getPositionY() - 166));
	button->addClickEventListener(CC_CALLBACK_1(OfficerView::ButtonCallback1, this));
	button->setTitleFontSize(32);
	this->addChild(button);
	//build dismiss button
	if (m_officer->getDefinition()->getName().find("captain") == std::string::npos)
	{
		button = ui::Button::create("art/bluebutton0.png", "art/bluebutton1.png", "art/bluebutton2.png");
		button->setTitleText("dismiss");
		button->addClickEventListener(CC_CALLBACK_1(OfficerView::ButtonCallback2, this));
		button->setPosition(Vec2(m_panel->getPositionX() + 194, m_panel->getPositionY() - 230));
		button->setTitleFontSize(32);
		this->addChild(button);
	}

	itoa(m_officer->getStat(0), &buffer[0], 10);
	str = "Attk:"; str.append(buffer);
	Label *label = Label::createWithBMFont("art/font.fnt", str);
	this->addChild(label);
	label->setPosition(m_panel->getPosition().x + 196, m_panel->getPositionY()-20);

	itoa(m_officer->getStat(1), &buffer[0], 10);
	str = "Def:"; str.append(buffer);
	label = Label::createWithBMFont("art/font.fnt", str);
	this->addChild(label);
	label->setPosition(m_panel->getPosition().x + 196, m_panel->getPositionY() - 50);

	itoa(m_officer->getStat(2), &buffer[0], 10);
	str = "Spcl:"; str.append(buffer);
	label = Label::createWithBMFont("art/font.fnt", str);
	this->addChild(label);
	label->setPosition(m_panel->getPosition().x + 196, m_panel->getPositionY() - 82);

	itoa(m_officer->getStat(3), &buffer[0], 10);
	str = "HP:"; str.append(buffer);
	label = Label::createWithBMFont("art/font.fnt", str);
	this->addChild(label);
	label->setPosition(m_panel->getPosition().x + 196, m_panel->getPositionY() - 112);

	int value = m_officer->getStat(4);
	itoa(value, &buffer[0], 10);
	str = "EP:"; str.append(buffer);
	label = Label::createWithBMFont("art/font.fnt", str);
	this->addChild(label);
	label->setPosition(m_panel->getPosition().x + 292, m_panel->getPositionY() - 112);
	

}



void OfficerView::BuildSubPanel()
{
	//build top frame
	m_subpanel = Sprite::create("art/frametall.png");
	m_subpanel->setPosition(m_panel->getPosition() + Vec2(-256, 0));
	m_subpanel->setScale(2);
	this->addChild(m_subpanel);

	//build labels
	Label *label = Label::createWithBMFont("art/font.fnt", "LEVEL UP AVAILABLE");
	label->setWidth(128);
	label->setScale(0.8F);
	label->setPosition(Vec2(64, 224));
	m_subpanel->addChild(label);
	//build improv values
	char buffer[3];
	itoa(m_officer->getDefinition()->getStat(4), &buffer[0], 10);
	std::string str = "ATK:"; str.append(buffer); str.append("+");
	label = Label::createWithBMFont("art/font.fnt", str);
	label->setPosition(32, 180);
	label->setScale(0.5F);
	m_subpanel->addChild(label);
	itoa(m_officer->getDefinition()->getStat(5), &buffer[0], 10);
	str = "DEF:"; str.append(buffer); str.append("+");
	label = Label::createWithBMFont("art/font.fnt", str);
	label->setPosition(96, 180);
	label->setScale(0.5F);
	m_subpanel->addChild(label);
	itoa(m_officer->getDefinition()->getStat(6), &buffer[0], 10);
	str = "SPE:"; str.append(buffer); str.append("+");
	label = Label::createWithBMFont("art/font.fnt", str);
	label->setPosition(32, 160);
	label->setScale(0.5F);
	m_subpanel->addChild(label);
	itoa(m_officer->getDefinition()->getStat(7), &buffer[0], 10);
	str = "HP:"; str.append(buffer); str.append("+");
	label = Label::createWithBMFont("art/font.fnt", str);
	label->setPosition(96, 160);
	label->setScale(0.5F);
	m_subpanel->addChild(label);


	m_button = ui::Button::create("art/bluebutton0.png", "art/bluebutton1.png", "art/bluebutton2.png");
	m_button->setTitleText("Level up");
	m_button->setPosition(Vec2(64, 16));
	m_button->addClickEventListener(CC_CALLBACK_1(OfficerView::ButtonCallback3, this));
	m_button->setTitleFontSize(32);
	m_button->setScaleY(0.5F);

	m_subpanel->addChild(m_button);

	int v = m_officer->getLevel()+1;
	if (v == 1 || v == 3 || v == 6)
	{
	

		//build ability selector
		label = Label::createWithBMFont("art/font.fnt", "ability selection");
		label->setPosition(64, 128);
		label->setAnchorPoint(Vec2(0.5F, 0.5F));
		label->setScale(0.75F);
		label->setWidth(128);
		m_subpanel->addChild(label);
		
		int offset = 1;
		if (v == 3)
		{
			offset = 3;
		}
		if (v == 6)
		{
			//build single box
			m_choiceframes[0] = Sprite::create("art/backbox2A.png", Rect(0, 32, 128, 32));
			m_choiceframes[0]->setPosition(m_subpanel->getPosition()+Vec2(0, -128+40));
			m_choiceframes[0]->setScale(2);
			this->addChild(m_choiceframes[0]);
			label = Label::createWithBMFont("art/font.fnt", m_officer->getDefinition()->getAbility(5)->getName());
			label->setPosition(m_choiceframes[0]->getPosition());
			this->addChild(label);
			label->setScale(0.5F);
			m_choiceframes[1] = NULL;
			m_selection = 1;
		}
		else
		{
			//build boxes
			for (int i = 0; i < 2; i++)
			{
				m_choiceframes[i] = Sprite::create("art/backbox2A.png", Rect(0, 0, 128, 32));
				m_choiceframes[i]->setPosition(m_subpanel->getPosition() + Vec2(0, -129 + 40 - (64 * i)));
				this->addChild(m_choiceframes[i]);
				m_choiceframes[i]->setScale(2);
				label = Label::createWithBMFont("art/font.fnt", m_officer->getDefinition()->getAbility(i + offset)->getName());
				label->setPosition(m_choiceframes[i]->getPosition());
				this->addChild(label);
				label->setScale(0.5F);
			}
			m_button->setEnabled(false);
			m_button->setBright(false);
		}

	}
	else
	{
		m_choiceframes[0] = NULL;
		m_choiceframes[1] = NULL;
	}


}

void OfficerView::onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event)
{
	Touch *touch = touches[0];

	//check if touching an ability
	for (int i = 3; i < 7; i++)
	{
		Vec2 p = touch->getLocation();
		Rect r = m_labelframes[i]->getBoundingBox();
	
		if (r.containsPoint(touch->getLocation()) && m_officer->getAbility(i-3)!=NULL)
		{
			m_description->setString(m_officer->getAbility(i - 3)->getDescription());
		}
	}



	if (m_panel->getBoundingBox().containsPoint(touch->getLocation())== false )
	{
		if (m_subpanel == NULL)
		{
			ReturnableLayer *layer = (ReturnableLayer*)m_layer;
			layer->ReturntoLayer();
			this->getParent()->removeChild(this);
		}
		else 
 		{
			if (m_subpanel->getBoundingBox().containsPoint(touch->getLocation()) == false && m_frame->getBoundingBox().containsPoint(touch->getLocation())==false)
			{
				ReturnableLayer *layer = (ReturnableLayer*)m_layer;
				layer->ReturntoLayer();
				this->getParent()->removeChild(this);
			}
			else
			{
				Vec2 p = touch->getLocation();
				//p = p + Vec2(64, 128);
				for (int i = 0; i < 2; i++)
				{
					if (m_choiceframes[i] != NULL)
					{
						Rect boundingbox = m_choiceframes[i]->getBoundingBox();
						if (m_choiceframes[i]->getBoundingBox().containsPoint(p))
						{
							m_choiceframes[0]->setTextureRect(Rect(0,0, 128, 32));
							m_choiceframes[1]->setTextureRect(Rect(0,0, 128, 32));
							m_selection = i ;
							m_choiceframes[i]->setTextureRect(Rect(0, 32, 128, 32));
							m_button->setBright(true);
							m_button->setEnabled(true);
							int v = m_officer->getLevel() + 1;
							if (v == 1)
							{
								m_description->setString(m_officer->getDefinition()->getAbility(1 + m_selection)->getDescription());
							}
							if (v == 3)
							{
								m_description->setString(m_officer->getDefinition()->getAbility(3 + m_selection)->getDescription());
							}
							if (v == 6)
							{
								m_description->setString(m_officer->getDefinition()->getAbility(5)->getDescription());
							}
						}
					}
				}
			}
		}
	}

}

void OfficerView::ButtonCallback0(Ref *pSender)
{
	//exit back
	ReturnableLayer *layer = (ReturnableLayer*)m_layer;
	layer->ReturntoLayer();
	this->getParent()->removeChild(this);

}

void OfficerView::ButtonCallback1(Ref *pSender)
{
	//rename
	RenamePanel *rpanel = RenamePanel::create();
	rpanel->Setup(m_officer->getNamePtr(), "Rename Officer", this);
	this->getParent()->addChild(rpanel);
	this->setVisible(false);
}

void OfficerView::ButtonCallback2(Ref *pSender)
{
	//dismiss
	
}

void OfficerView::ButtonCallback3(Ref *pSender)
{
	//level up
	m_officer->LevelUp(m_selection);
	//dump and rebuild officerview
	OfficerView *view = OfficerView::create();
	view->Setup(m_officer, m_layer, m_description);
	this->getParent()->addChild(view);
	this->getParent()->removeChild(this);
	view->setTouchEnabled(true);
}

void OfficerView::ButtonCallback4(Ref *pSender)
{

}

void OfficerView::ReturntoLayer()
{
	this->setVisible(true);
	m_labels[0]->setString(m_officer->getName());
}