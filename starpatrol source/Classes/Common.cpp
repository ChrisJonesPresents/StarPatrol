#include "Common.h"


MStageType StrtoStageType(std::string str)
{
	if (str.find("AWAY") != std::string::npos)
	{
		return MST_AWAYTEAM;
	}
	if (str.find("BATTLE") != std::string::npos)
	{
		return MST_BATTLE;
	}
	if (str.find("CHOICE") != std::string::npos)
	{
		return MST_CHOICE;
	}
	if (str.find("TEXT") != std::string::npos)
	{
		return MST_TEXT;
	}
	if (str.find("DIALOGUE") != std::string::npos)
	{
		return MST_DIALOGUE;
	}
	if (str.find("REWARD") != std::string::npos)
	{
		return MST_REWARD;
	}
	if (str.find("COMPLETE") != std::string::npos)
	{
		return MST_COMPLETE;
	}
	if (str.find("FAIL") != std::string::npos)
	{
		return MST_FAIL;
	}
	if (str.find("PUNISH") != std::string::npos)
	{
		return MST_PUNISH;
	}
	if (str.find("GAMEOVER") != std::string::npos)
	{
		return MST_GAMEOVER;
	}
	if (str.find("EXPLORE") != std::string::npos)
	{
		return MST_EXPLORE;
	}
	if (str.find("TRADE") != std::string::npos)
	{
		return MST_TRADE;
	}

}

Vec2 getTouchpos(Vec2 p,Layer *l)
{
	p.y -= 640;
	p.y = p.y*-1; 
	cocos2d::Vec2 s=Director::getInstance()->getVisibleSize();
	if (s.x > 800)
	{
		p.x = p.x -((s.x - 800)*0.5F*l->getScaleX());
	}

	s = s*l->getScale();
	return p;
}


Shipmod StrtoShipmod(std::string str)
{
	if (str.find("FUEL") != std::string::npos)
	{
		return SM_fuelcap;
	}
	if (str.find("CREW") != std::string::npos)
	{
		return SM_crewcap;
	}
	if (str.find("HULL") != std::string::npos)
	{
		return SM_hpcap;
	}
	if (str.find("FTL") != std::string::npos)
	{
		return SM_speed;
	}
	if (str.find("DEFLECT") != std::string::npos)
	{
		return SM_deflect;
	}
	if (str.find("INTERCEPT") != std::string::npos)
	{
		return SM_intercept;
	}
	if (str.find("DODGE") != std::string::npos)
	{
		return SM_dodge;
	}
	if (str.find("BEAM") != std::string::npos)
	{
		return SM_BEAM;
	}
	if (str.find("MISSILE") != std::string::npos)
	{
		return SM_MISSILE;
	}
	if (str.find("PROJECTILE") != std::string::npos)
	{
		return SM_PROJECTILE;
	}
}

FieldType StrtoFieldType(std::string str)
{
	if (str.find("REDSHIRT") != std::string::npos)
	{
		return FT_REDSHIRT;
	}
	if (str.find("ALIEN") != std::string::npos)
	{
		return FT_ALIEN;
	}
	if (str.find("BEAST") != std::string::npos)
	{
		return FT_BEAST;
	}
	if (str.find("ROBOT") != std::string::npos)
	{
		return FT_ROBOT;
	}
	if (str.find("ORGANIC") != std::string::npos)
	{
		return FT_ORGANIC;
	}
	if (str.find("ALL") != std::string::npos)
	{
		return FT_ALL;
	}
}

TargetType StrtoTargettype(std::string str)
{
	if (str.find("ANY") != std::string::npos)
	{
		return TT_EITHER;
	}
	if (str.find("BACK") != std::string::npos)
	{
		return TT_BACK;
	}
	if (str.find("FRONT") != std::string::npos)
	{
		return TT_FRONT;
	}
	if (str.find("MOB") != std::string::npos)
	{
		return TT_MOB;
	}
	if (str.find("SINGLE") != std::string::npos)
	{
		return TT_SINGLE;
	}
		
}


FieldMod StrtoFieldmod(std::string str)
{
	if (str.find("FM_ATTACK") != std::string::npos)
	{
		return FM_ATTACK;
	}
	if (str.find("FM_DEFENCE") != std::string::npos)
	{
		return FM_DEFENCE;
	}


}

FieldAttack StrotoFieldattack(std::string str)
{
	if (str.find("ENERGY") != std::string::npos)
	{
		return FA_ENERGY;
	}
	if (str.find("IMPACT") != std::string::npos)
	{
		return FA_IMPACT;
	}
	if (str.find("EXPLOSION") != std::string::npos)
	{
		return FA_EXPLOSION;
	}
}

std::string myreplace(std::string s,
	std::string toReplace,
	std::string replaceWith)
{
	std::string str = s.replace(s.find(toReplace), toReplace.length(), replaceWith);
	return str;
}

void SaveLongString(std::string str, FILE *file)
{
	int check = str.length();
	char *buffer = new char[check + 3];
	memcpy((void*)&buffer[0], (void*)str.c_str(), str.length());
	buffer[check] = ' ';
	buffer[check + 1] = '#';
	buffer[check + 2] = ' ';
	fwrite((void*)&buffer[0], sizeof(char), check + 3, file);
}

void SaveShortString(std::string str, FILE *file)
{
	int check = str.length();
	char *buffer = new char[str.length() + 1];
	memcpy((void*)&buffer[0], (void*)str.c_str(), str.length());
	buffer[str.length()] = ' ';
	fwrite((void*)&buffer[0], sizeof(char), str.length() + 1, file);
}

std::string LoadLongString(CBReader *reader)
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
			str.append(input);
			str.append(" ");
		}
	}



	return str;
}

void ProgressReader(CBReader *reader, int value)
{
	while (true)
	{

		std::string str = reader->ReadString();
		if (str.find("TEXT") != std::string::npos)
		{
			int v = reader->ReadInt();
			if (v == value)
			{
				break;
			}
		}
	}
}