#ifndef COLONY
#define COLONY
#include "ShopDef.h"
#include "Facility.h"
#include <string>
enum ColonyType {CT_BARREN,CT_ICE,CT_HELL,CT_GARDEN};
class Colony
{
	Colony *m_neighbours[4];
	unsigned int m_tradelevel;
	std::string m_name;
	ColonyType m_type;
	ShopDef *m_def;
	Facility *m_facilities[8];
	bool m_facilityrunning[8];
	int m_defence[2];
	unsigned int m_resources[8][4];
	//0 is resources made locally
	//1 is trade resources in use here
	//2 is local resource use
	//3 is our resource used in other colonies
	void AddModule(short category);
	void RemoveModule(short category);
	void AddItem(short Item);
	void RemoveItem(short Item);
	void AddOutput(int type, int value);
	void ResourceStarvation(int i);
	void ResourceRestoration(int i);
	void ActivateFacility(int i);
	void DeactivateFacility(int i);

public:

	Colony(ColonyType type, std::string name);
	
	void Save(FILE *file);
	Colony(CBReader *reader);
	~Colony();
	std::string *getName();
	ShopDef *getDef();
	ColonyType getType();
	Facility *getFacility(int i);
	unsigned int getResMax(int i);
	unsigned int getResFree(int i);

	void BuildFacility(Facility *newfac, int slot);
	void UpgradeFacility(Facility *newfac, int slot);
	void DestroyFacility(int slot);
	void setRunning(int i, bool b);
	bool getRunning(int i);
	bool SanityCheck();
	bool HasFacilities();
	//trade segment
	void AddColony(Colony *colony);
	void RemoveColony(Colony *colony);
	int getTradeRes(int i);
	int getTradeResUsed(int i);
	int getResFromTrade(int i);
	int getAvailableRes(int i);
	int ReturnTradeRes(int i, int amount);
	void UseTradeRes(int i, int amount);
	void RefundRes(int i, int amount);
	void InvestRes(int i, int amount);
	int DepriveRes(int i, int amount);
	int getUsedRes(int i);
	void RestartCheck();
	void Restock();
	int getDefence(int i);
	int getDefenceBonus();
};

#endif