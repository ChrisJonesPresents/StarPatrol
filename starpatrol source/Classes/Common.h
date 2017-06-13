#ifndef COMMON
#define COMMON
#define byte uint8_t
#include "cocos2d.h"
#include "CBReader.h"
USING_NS_CC;

#define STAT_ATTACK 0
#define STAT_DEFENCE 1
#define STAT_SPECIAl 2
#define STAT_HEALTH 3
#define GROWTH_ATTACK 4
#define GROWTH_DEFENCE 5
#define GROWTH_SPECIAL 6
#define GROWTH_HEALTH 7

Vec2 getTouchpos(Vec2 p, Layer *l);

enum AwayAbilityType{ AAT_ATTACK, AAT_BUFF, AAT_DEBUFF, AAT_IMMUNIZE, AAT_RECOVER, AAT_NONE };
enum AwayAbilityTargetRule{ AATR_SELF, AATR_ENEMY, AATR_ALLY, AATR_ENEMIES, AATR_ALLIES};
enum AwayModType{ AMT_ATTACK, AMT_DEFENCE_EXP, AMT_DEFENCE_IMP, AMT_DEFENCE_ENE, AMT_SPECIAL, AMT_STUN, AMT_ENERGY, AMT_HEALTH, AMT_EOT_HEALTH, AMT_EOT_ENERGY, AMT_TARGETRATE };

enum FieldAttack{FA_ENERGY, FA_IMPACT, FA_EXPLOSION,FA_ENEMY};

enum Shipmod{SM_fuelcap,SM_crewcap,SM_hpcap,SM_speed,SM_deflect,SM_intercept,SM_dodge,SM_BEAM,SM_MISSILE,SM_PROJECTILE};

enum FieldType{FT_ALL,FT_REDSHIRT, FT_ALIEN, FT_BEAST, FT_ROBOT,FT_ORGANIC };

enum TargetType{TT_EITHER, TT_FRONT, TT_BACK,TT_MOB,TT_SINGLE};

enum FieldMod{FM_ATTACK, FM_DEFENCE,FM_STUN};

enum MissionReward { MR_CREDITS, MR_MODULE, MR_OFFICER };

enum MStageType{ MST_TEXT, MST_CHOICE, MST_DIALOGUE, MST_AWAYTEAM, MST_BATTLE,MST_REWARD,MST_COMPLETE,MST_FAIL,MST_PUNISH,MST_GAMEOVER,MST_EXPLORE,MST_TRADE};

MStageType StrtoStageType(std::string str);

Shipmod StrtoShipmod(std::string str);

FieldType StrtoFieldType(std::string str);

TargetType StrtoTargettype(std::string str);

FieldMod StrtoFieldmod(std::string str);

FieldAttack StrotoFieldattack(std::string str);

std::string myreplace(std::string s,
	std::string toReplace,
	std::string replaceWith);

void SaveLongString(std::string str, FILE *file);
void SaveShortString(std::string str, FILE *file);
std::string LoadLongString(CBReader *reader);


void ProgressReader(CBReader *reader, int value);
#endif