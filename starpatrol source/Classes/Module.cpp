#include "Module.h"

void Module::CommonLoad(CBReader *reader)
{
	//load name 
	m_name = "";
	while (true)
	{
		std::string str = reader->ReadString();
		if (str.find("#") != std::string::npos)
		{
			break;
		}
		else
		{
			m_name.append(str + " ");
		}
	}
	m_description = "";
	while (true)
	{
		std::string str = reader->ReadString();
		if (str.find("#") != std::string::npos)
		{
			break;
		}
		else
		{
			m_description.append(str + " ");
		}
	}

	//load graphicspos
	m_graphicpos = cocos2d::Vec2(reader->ReadInt(), reader->ReadInt());
	//load type
	m_category = reader->ReadInt();
	//load cost
	m_cost = reader->ReadInt();
}

Module::Module()
{

}

std::string Module::getName()
{
	return m_name;
}

std::string Module::getDesc()
{
	return m_description;
}

int Module::getCost()
{
	return m_cost;
}

ModuleType Module::getType()
{
	return m_type;
}

cocos2d::Vec2 Module::getGraphicpos()
{
	return m_graphicpos; 
}

short Module::getCategory()
{
	return m_category;
}

int Module::getUID()
{
	return m_uid;
}
void Module::setUID(int id)
{
	m_uid = id;
}