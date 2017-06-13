#include "RenamePanel.h"
#include <ui\UIButton.h>

void RenamePanel::Setup(std::string *string,std::string prompt, ReturnableLayer *main)
{
	m_main = main;
	m_string = string;
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//build frame
	Sprite *frame = Sprite::create("art/frame.png");
	frame->setPosition(Vec2(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height / 2)));
	this->addChild(frame);
	frame->setScale(2);

	//build prompt
	Label *label = Label::createWithBMFont("art/font.fnt", prompt);
	label->setPosition(Vec2(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height / 2)+64));
	this->addChild(label);

	m_backupstring = *string;
	m_string = string;
	//build text field
	m_textfield = ui::TextField::create();
	this->addChild(m_textfield);
	m_textfield->setPosition(Vec2(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height / 2)));
	m_textfield->setString(*m_string);

	//build confirm button
	ui::Button *button = ui::Button::create("art/bluebutton0.png", "art/bluebutton1.png", "art/bluebutton2.png");
	button->setTitleText("Confirm");
	button->setPosition(Vec2(frame->getPositionX()-64,frame->getPositionY()-48));
	button->setTitleFontSize(24);
	button->addClickEventListener(CC_CALLBACK_1(RenamePanel::ButtonCallback0, this));
	this->addChild(button);
	//build cancel button
	button = ui::Button::create("art/bluebutton0.png", "art/bluebutton1.png", "art/bluebutton2.png");
	button->setTitleText("Cancel");
	button->setPosition(Vec2(frame->getPositionX() + 64, frame->getPositionY() - 48));
	button->setTitleFontSize(24);
	button->addClickEventListener(CC_CALLBACK_1(RenamePanel::ButtonCallback1, this));
	this->addChild(button);
}
void RenamePanel::ButtonCallback0(Ref *pSender)
{
	//confirm button
	*m_string = m_textfield->getString();
	this->m_main->setVisible(true);
	this->m_main->ReturntoLayer();
	this->getParent()->removeChild(this);
}

void RenamePanel::ButtonCallback1(Ref *pSender)
{
	//cancel button

	this->m_main->setVisible(true);
	this->m_main->ReturntoLayer();
	this->getParent()->removeChild(this);
}