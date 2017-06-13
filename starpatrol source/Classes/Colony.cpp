#include "Colony.h"
#include "ReferenceLibrary.h"



Colony::Colony(ColonyType type, std::string name)
{
	m_type = type;
	m_name = name;

	m_tradelevel = 0;

	m_defence[0] = 0;
	m_defence[1] = 0;

	for (int i = 0; i < 8; i++)
	{
		m_facilities[i] = NULL;
		m_resources[i][0] = 0;
		m_resources[i][1] = 0;
		m_resources[i][2] = 0;
		m_resources[i][3] = 0;
		m_facilityrunning[i] = true;
	}
	for (int i = 0; i < 4; i++)
	{
		m_neighbours[i] = NULL;
	}

	m_def = new ShopDef();
}

void Colony::Save(FILE *file)
{
	
	fwrite((void*)&m_type, sizeof(int), 1, file);
	SaveLongString(m_name, file);
	
	fwrite((void*)&m_tradelevel, sizeof(int), 1, file);
	for (int i = 0; i < 2; i++)
	{
		fwrite((void*)&m_defence[i], sizeof(int), 1, file);
	}

	for (int i = 0; i < 8; i++)
	{
		if (m_facilities[i] != NULL)
		{
			char v = 42;
			fwrite((void*)&v, sizeof(char), 1, file);
			int value = m_facilities[i]->getID();
			fwrite((void*)&value, sizeof(int), 1, file);
		}
		else
		{
			char v = 17;
			fwrite((void*)&v, sizeof(char), 1, file);
		}
		for (int j = 0; j < 4; j++)
		{
			fwrite((void*)&m_resources[i][j], sizeof(int), 1, file);
		}
		fwrite((void*)&m_facilityrunning[i], sizeof(bool), 1, file);
	}

	m_def->Save(file);
}

Colony::Colony(CBReader *reader)
{
	m_type = (ColonyType)reader->ReadBInt();
	m_name = LoadLongString(reader);
	m_tradelevel = reader->ReadBInt();
	for (int i = 0; i < 2; i++)
	{
		m_defence[i] = reader->ReadBInt();
	}
	
	for (int i = 0; i < 8; i++)
	{
		char v = reader->ReadChar();
		if (v == 42)
		{
			int value = reader->ReadBInt();
			m_facilities[i]=ReferenceLibrary::getInstance()->getFacility(value);
		}
		else
		{
			m_facilities[i] = NULL;
		}
		for (int j = 0; j < 4; j++)
		{
			m_resources[i][j] = reader->ReadBInt();
		}
		m_facilityrunning[i] = reader->ReadChar();
	}

	m_def = new ShopDef(reader);

	for (int i = 0; i < 4; i++)
	{
		m_neighbours[i] = NULL;
	}
}

bool Colony::HasFacilities()
{
	for (int i = 0; i < 8; i++)
	{
		if (m_facilities[i] != NULL)
		{
			return true;
		}
	}
	return false;
}
void Colony::AddColony(Colony *colony)
{
	for (int i = 0; i < 4; i++)
	{
		if (m_neighbours[i] == NULL)
		{
			m_neighbours[i] = colony;
			break;
		}
	}
}

void Colony::RemoveColony(Colony *colony)
{
	for (int i = 0; i < 4; i++)
	{
		if (m_neighbours[i] == colony)
		{
			m_neighbours[i] = NULL;
		}
	}
}

Colony::~Colony()
{
	delete m_def;
}

ColonyType Colony::getType()
{
	return m_type;
}

std::string *Colony::getName()
{
	return &m_name;
}

ShopDef *Colony::getDef()
{
	return m_def;
}

Facility *Colony::getFacility(int i)
{
	return m_facilities[i];
}

unsigned int Colony::getResMax(int i)
{
	return m_resources[i][0];
}



int Colony::getResFromTrade(int i)
{
	int v = 0;
	for (int j = 0; j < 4; j++)
	{
		if (m_neighbours[j] != NULL)
		{
			v += m_neighbours[j]->getTradeRes(i);
		}
	}
	return v;
}


int Colony::getAvailableRes(int i)
{
	int v = getResFromTrade(i);
	return v + m_resources[i][0] - m_resources[i][2] - m_resources[i][3];

}

void Colony::RefundRes(int i, int amount)
{
	//first check if we can refund trade res
	if (m_resources[i][1] > 0)
	{
		for (int k = 0; k < 4; k++)
		{
			if (m_neighbours[k] != NULL)
			{
				int v = m_neighbours[k]->ReturnTradeRes(i, amount);
				m_resources[i][1] -= v;
				amount -= v;
				
			}
		}
	}
	//next, refund existing res
	if (amount>0)
	{
		m_resources[i][2] -= amount;
	}
}

int Colony::getUsedRes(int i)
{
	return m_resources[i][1] + m_resources[i][2] + m_resources[i][3];
}

void Colony::InvestRes(int i, int amount)
{
	//first do we have enough local res to use
	if (m_resources[i][0] - m_resources[i][2] - m_resources[i][3]>0)
	{
		if (m_resources[i][0] - m_resources[i][2] - m_resources[i][3] >= amount)
		{
			m_resources[i][2] += amount;
			amount = 0;
		}
		else
		{
			amount = amount - (m_resources[i][0] - m_resources[i][2] - m_resources[i][3]);
			m_resources[i][2] += (m_resources[i][0] - m_resources[i][2] - m_resources[i][3]);
		}
	}
	//next, check if we have trade res sufficient
	if (amount > 0)
	{
		for (int j = 0; j < 4; j++)
		{
			if (m_neighbours[j] != NULL)
			{
				int v = m_neighbours[j]->getTradeRes(i);
				if (v>0)
				{
					if (v >= amount)
					{
						m_neighbours[j]->UseTradeRes(i, amount);
						m_resources[i][1] = amount;
						amount = 0;
					}
					else
					{
						m_neighbours[j]->UseTradeRes(i, v);
						m_resources[i][1] = v;
						amount = amount - v;
					}

				}
			}
			if (amount <= 0)
			{
				break;
			}
		}
	}
}

int Colony::ReturnTradeRes(int i, int amount)
{
	if (m_resources[i][3] >= amount)
	{
		m_resources[i][3] -= amount;
		return amount;
	}
	else
	{
		int v = m_resources[i][3];
		m_resources[i][3] = 0;
		return v;
	}

}

void Colony::UseTradeRes(int i, int amount)
{
	m_resources[i][3] += amount;
}



unsigned int Colony::getResFree(int i)
{
	int v = getResFromTrade(i);
	return m_resources[i][0] - m_resources[i][2]-m_resources[i][3];
}

void Colony::BuildFacility(Facility *newfac, int slot)
{
	//place new structure
	m_facilities[slot] = newfac;
	ActivateFacility(slot);
}

int Colony::getTradeRes(int i)
{
	if (i == 0)
	{
		return 0;
	}
	if (m_tradelevel > 0)
	{
		//calculate local res
		return 	m_resources[i][0] - m_resources[i][3] - m_resources[i][2];
	}
	return 0;
}

int Colony::getTradeResUsed(int i)
{
	return m_resources[i][1];
}

void Colony::AddOutput(int type, int value)
{
	switch (type)
	{
		//services

		case -1:
			//module
			//add a new module
			AddModule(value);
			break;

		case -2:
			//repair
			m_def->m_supplies[0][1] += value;
			m_def->m_supplies[0][0] = 4;
			break;

		case -3:
			//fuel
			m_def->m_supplies[1][1] += value;
			m_def->m_supplies[1][0] = 2;
			break;

		case -4:
			//ammo
			m_def->m_supplies[2][1] += value;
			m_def->m_supplies[2][0] = 4;
			break;

		case -5:
			//allyship
			break;

		case -6:
			//officer
			break;

			//resources
		case -7:
			AddItem(value);
			break;

		case 1:
			m_resources[0][0] += value;
			ResourceRestoration(0);
			break;

		case 2:
			m_resources[1][0] += value;
			if (m_type == CT_BARREN)
			{
				m_resources[1][0] += 1;
			}
			ResourceRestoration(1);
			break;

		case 3:
			m_resources[2][0] += value;
			if (m_type ==CT_HELL)
			{
				m_resources[2][0] += 1;
			}
			ResourceRestoration(2);
			break;

		case 4:
			m_resources[3][0] += value;
			if (m_type == CT_GARDEN)
			{
				m_resources[3][0] += 1;
			}
			ResourceRestoration(3);
			break;
		case 5:
			m_resources[4][0] += value;

			ResourceRestoration(4);
			break;

		case 6:
			m_resources[5][0] += value;
			if (m_type == CT_ICE)
			{
				m_resources[5][0] += 1;
			}
			ResourceRestoration(5);
			break;

		case 7:
			m_resources[6][0] += value;
			ResourceRestoration(6);
			break;

		case 8:
			m_resources[7][0] += value;
			ResourceRestoration(7);
			break;

		case 10:
			if (m_tradelevel < value)
			{
				m_tradelevel = value;
			}
			break;
		case 11:
			m_defence[0] += value;
			break;
		case 12:
			m_defence[1] += value;
			break;
		}
	

}


void Colony::UpgradeFacility(Facility *newfac, int slot)
{
	//refund old resource cost
	//spend new resource cost
	for (int i = 0; i < 8; i++)
	{
		RefundRes(i,m_facilities[slot]->getCost(i));
		InvestRes(i, newfac->getCost(i));
	}
	
	for (int i = 0; i < 3; i++)
	{
		//compare with existing service

		//if new add as normal
		if (m_facilities[slot]->getOutputType(i) == 0 && newfac->getOutputType(i) != 0)
		{
			AddOutput(newfac->getOutputType(i), newfac->getOutputValue(i));
		}
		//make sure upgrades never remove or change the order of things
		if (m_facilities[slot]->getOutputType(i) == newfac->getOutputType(i))
		{
			//if same and service, get difference
			if (newfac->getOutputType(i)<-1 && newfac->getOutputType(i)>-5)
			{
				int d = newfac->getOutputValue(i) - m_facilities[slot]->getOutputValue(i);
				m_def->m_supplies[(newfac->getOutputType(i)*-1) - 1][1] += d;
			}
				//if same and resource, get difference
			if (newfac->getOutputType(i)>0)
			{
				int d = newfac->getOutputValue(i) - m_facilities[slot]->getOutputValue(i);
				m_resources[newfac->getOutputType(i) - 1][0] += d;
			}
		}

	}
	m_facilities[slot] = newfac;
	
}

void Colony::DestroyFacility(int slot)
{
	DeactivateFacility(slot);
	m_facilities[slot] = NULL;
}

void Colony::AddModule(short category)
{
	int v = m_def->m_numitems + 1;

	int *nlist0 = new int[v];
	int *nlist1 = new int[v];

	int item = ReferenceLibrary::getInstance()->getInventoryModule(category);
	int safety = 3;
	while (m_def->PreventDupe(2, item) && safety>0)
	{
		//reroll
		item=ReferenceLibrary::getInstance()->getInventoryModule(category);
		safety--;
	}
	if (m_def->m_numitems > 0)
	{
		for (int i = 0; i < m_def->m_numitems; i++)
		{
			nlist0[i] = m_def->m_items[0][i];
			nlist1[i] = m_def->m_items[1][i];
		}
		delete [] m_def->m_items[0];
		delete [] m_def->m_items[1];
	}
	nlist0[v - 1] = 1; 
	nlist1[v - 1] = item;
	m_def->m_numitems = v;
	m_def->m_items[0] = nlist0;
	m_def->m_items[1] = nlist1;
}

void Colony::AddItem(short category)
{
	int v = m_def->m_numrecruits + 1;
	int *nlist0 = new int[v];
	int *nlist1 = new int[v];
	int item = ReferenceLibrary::getInstance()->getInventoryPotion(category);
	int safety = 3;
	while (m_def->PreventDupe(3, item) && safety>0)
	{
		//reroll
		item = ReferenceLibrary::getInstance()->getInventoryPotion(item);
		safety--;
	}
	if (m_def->m_numitems > 0)
	{
		for (int i = 0; i < m_def->m_numitems; i++)
		{
			nlist0[i] = m_def->m_recruits[0][i];
			nlist1[i] = m_def->m_recruits[1][i];
		}
		delete[] m_def->m_recruits[0];
		delete[] m_def->m_recruits[1];
	}
	nlist0[v - 1] = 1;
	nlist1[v - 1] = item;
	m_def->m_numrecruits = v;
	m_def->m_recruits[0] = nlist0;
	m_def->m_recruits[1] = nlist1;
}

void Colony::RemoveItem(short category)
{
	ReferenceLibrary *library = ReferenceLibrary::getInstance();

	int v = m_def->m_numrecruits - 1;
	if (v == 0)
	{

		delete[] m_def->m_recruits[0];
		delete[] m_def->m_recruits[1];
	}
	else
	{
		int *cat0 = new int[v];
		int *cat1 = new int[v];

		//create a new list

		bool drop = false;
		for (int i = 0; i < m_def->m_numrecruits; i++)
		{
			if (drop == true)
			{
				cat0[i - 1] = m_def->m_recruits[0][i];
				cat1[i - 1] = m_def->m_recruits[1][i];
			}
			else
			{
				if (m_def->m_recruits[0][i] == 1)
				{
					if (library->getPotion(m_def->m_recruits[1][i])->m_category == category)
					{
						drop = true;
					}
					else
					{
						cat0[i] = m_def->m_recruits[0][i];
						cat1[i] = m_def->m_recruits[1][i];
					}
				}

			}
		}
		delete[] m_def->m_recruits[0];
		delete[] m_def->m_recruits[1];

		m_def->m_recruits[0] = cat0;
		m_def->m_recruits[1] = cat1;
	}

	m_def->m_numrecruits = v;
}

void Colony::RemoveModule(short category)
{

	ReferenceLibrary *library = ReferenceLibrary::getInstance();

	int v = m_def->m_numitems - 1;
	if (v == 0)
	{

		delete [] m_def->m_items[0];
		delete[] m_def->m_items[1];
	}
	else
	{
		int *cat0 = new int[v];
		int *cat1 = new int[v];

		//create a new list

		bool drop = false;
		for (int i = 0; i < m_def->m_numitems; i++)
		{
			if (drop == true)
			{
				cat0[i - 1] = m_def->m_items[0][i];
				cat1[i - 1] = m_def->m_items[1][i];
			}
			else
			{
				if (m_def->m_items[0][i] == 2)
				{
					if (library->getModule(m_def->m_items[1][i])->getCategory() == category)
					{
						drop = true;
					}
					else
					{
						cat0[i] = m_def->m_items[0][i];
						cat1[i] = m_def->m_items[1][i];
					}
				}

			}
		}
		delete[] m_def->m_items[0];
		delete[] m_def->m_items[1];

		m_def->m_items[0] = cat0;
		m_def->m_items[1] = cat1;
	}

	m_def->m_numitems = v;
}

void Colony::setRunning(int i, bool b)
{
	m_facilityrunning[i] = b;

}

bool Colony::getRunning(int i)
{
	return m_facilityrunning[i];
}

bool Colony::SanityCheck()
{
	bool b = false;
	for (int i = 0; i < 8; i++)
	{
		int r = m_resources[i][0]+m_resources[i][1] - m_resources[i][2] - m_resources[i][3];
		//if onsite and trade res exceeds onsite production we have resource starvation
		if (r < 0)
		{
			b = true;
			//figure out which facility here is using it
			ResourceStarvation(i);

		}
	}
	return b;
}

void Colony::ResourceStarvation(int i)
{
	int amount = m_resources[i][2] + m_resources[i][3] - m_resources[i][0]-m_resources[i][1];
	//first check if we're exporting this resource anywhere
	if (m_resources[i][3] > 0)
	{
		//then cut them off
		for (int j = 0; j<4; j++)
		{
			if (m_neighbours[j] != NULL)
			{
				amount = m_neighbours[j]->DepriveRes(i, amount);
			}
			if (amount <= 0)
			{
				break;
			}
		}

	}
	if (amount > 0)
	{
		for (int j = 0; j < 8; j++)
		{
			if (m_facilityrunning[j] == true && m_facilities[j] != NULL)
			{
				if (m_facilities[j]->getCost(i)>0)
				{
					amount = amount - m_facilities[j]->getCost(i);
					DeactivateFacility(i);

				}
			}
			if (amount <= 0)
			{
				break;
			}
		}
	}
}

int Colony::DepriveRes(int i, int amount)
{
	if (m_resources[i][1] > 0)
	{
		//cycle through structures
		for (int j = 0; j < 8; j++)
		{
			if (m_facilities[j] != NULL && m_facilityrunning[i] == true)
			{
				//does the structure use this resource?
				if (m_facilities[j]->getCost(i)>0)
				{
					//shut it down
					amount -= m_facilities[j]->getCost(i);
					DeactivateFacility(j);
				}
			}
			if (amount <= 0)
			{
				break;
			}
		}
	}
	if (amount < 0)
	{
		amount = 0;
	}
	if (m_resources[i][1] < 0)
	{
		m_resources[i][1] = 0;
	}
	return amount;
}

void Colony::DeactivateFacility(int i)
{
	m_facilityrunning[i] = false;
	Facility *olfac = m_facilities[i];
	//increase resource uses
	for (int i = 0; i < 8; i++)
	{
		if (olfac->getCost(i)>0)
		{
			RefundRes(i, olfac->getCost(i));
		}

	}

	//remove services
	for (int i = 0; i < 3; i++)
	{
		switch (olfac->getOutputType(i))
		{
			//services

		case -1:
			//module
			//remove a module with the right category tag from the shop
			RemoveModule(olfac->getOutputValue(i));
			break;

		case -2:
			//repair
			m_def->m_supplies[0][1] -= olfac->getOutputValue(i);
			if (m_def->m_supplies[0][1] < 0)
			{
				m_def->m_supplies[0][1] = 0;
			}
			break;

		case -3:
			//fuel
			m_def->m_supplies[1][1] -= olfac->getOutputValue(i);
			if (m_def->m_supplies[1][1] < 0)
			{
				m_def->m_supplies[1][1] = 0;
			}
			break;

		case -4:
			//ammo
			m_def->m_supplies[2][1] -= olfac->getOutputValue(i);
			if (m_def->m_supplies[2][1] < 0)
			{
				m_def->m_supplies[2][1] = 0;
			}
			break;

		case -5:
			//allyship
			break;

		case -6:
			//officer
			break;

		case -7:
			RemoveItem(olfac->getOutputValue(i));
			break;
			//resources
		case 1:
			m_resources[0][0] -= olfac->getOutputValue(i);
			break;

		case 2:
			m_resources[1][0] -= olfac->getOutputValue(i);
			if (m_type == CT_BARREN)
			{
				m_resources[1][0] -= 1;
			}
			break;

		case 3:
			m_resources[2][0] -= olfac->getOutputValue(i);
			if (m_type == CT_HELL)
			{
				m_resources[2][0] -= 1;
			}
			break;

		case 4:
			m_resources[3][0] -= olfac->getOutputValue(i);
			if (m_type == CT_GARDEN)
			{
				m_resources[3][0] -= 1;
			}
			break;
		case 5:
			m_resources[4][0] -= olfac->getOutputValue(i);
			break;

		case 6:
			m_resources[5][0] -= olfac->getOutputValue(i);
			if (m_type == CT_ICE)
			{
				m_resources[5][0] -= 1;
			}
			break;

		case 7:
			m_resources[6][0] -= olfac->getOutputValue(i);
			break;

		case 8:
			m_resources[7][0] -= olfac->getOutputValue(i);
			break;



		case 10:
			m_tradelevel = 0;
			for (int j = 0; j < 8; j++)
			{
				if (m_resources[j][3]>0)
				{
					int amount = m_resources[j][3];
					for (int k = 0; k < 4; k++)
					{
						if (m_neighbours[k] != NULL)
						{
							amount = m_neighbours[k]->DepriveRes(j, amount);
						}
						if (amount <= 0)
						{
							break;
						}
					}
				}
			}
			break;
		case 11:
			m_defence[0] -= olfac->getOutputValue(i);;
			break;
		case 12:
			m_defence[1] -= olfac->getOutputValue(i);;
			break;

		}

	}



}

void Colony::ResourceRestoration(int i)
{
	for (int i = 0; i < 8; i++)
	{
		if (m_facilityrunning[i] == false && m_facilities[i]!=NULL)
		{
			bool canactivate = true;
			for (int j = 0; j < 8; j++)
			{
				if (m_facilities[i]->getCost(j)>getResFree(j))
				{
					canactivate = false;
				}
			}
			if (canactivate == true)
			{
				ActivateFacility(i);
			}
		}
	}
}

void Colony::ActivateFacility(int i)
{
	m_facilityrunning[i] = true;
	Facility *newfac = m_facilities[i];
	//increase resource uses
	for (int i = 0; i < 8; i++)
	{
		if (newfac->getCost(i)>0)
		{
			InvestRes(i, newfac->getCost(i));
		}

	}

	for (int i = 0; i < 3; i++)
	{
		if (newfac->getOutputType(i) != 0)
		{
			AddOutput(newfac->getOutputType(i), newfac->getOutputValue(i));
		}
	}
}

void Colony::RestartCheck()
{
	for (int i = 0; i < 8; i++)
	{
		if (m_facilityrunning[i] == false && m_facilities[i]!=NULL)
		{
			bool b = true;
			for (int j = 0; j < 8; j++)
			{
				if (getAvailableRes(j) < m_facilities[i]->getCost(j))
				{
					b = false;
				}
			}
			if (b == true)
			{
				ActivateFacility(i);
			}
		}
	}
}

void Colony::Restock()
{
	//set stock to zero
	for (int i = 0; i < 4; i++)
	{
		m_def->m_supplies[i][1] = 0;
	}
	//restock
	for (int i = 0; i < 8; i++)
	{
		if (m_facilities[i] != NULL && m_facilityrunning[i]==true)
		{
			Facility *facility = m_facilities[i];
			for (int j = 0; j < 3; j++)
			{
				switch (facility->getOutputType(j))
				{
				case -2:
					//repair
					m_def->m_supplies[0][0] += facility->getOutputValue(j);
					break;
				case 3:
					//fuel
					m_def->m_supplies[1][0] += facility->getOutputValue(j);
					break;
				case 4:
					//ammo
					m_def->m_supplies[2][0] += facility->getOutputValue(j);
					break;

				}
			}
		}
	}

	//shop reset
	if (m_def->m_numitems > 0)
	{
		int c = 0;
		if (m_def->m_numitems > 1)
		{
			c = rand() % m_def->m_numitems;
		}
		int cat = 0;
		if (m_def->m_items[0][c] == 2)
		{
			cat = ReferenceLibrary::getInstance()->getModule(m_def->m_items[1][c])->getCategory();
		}
		int item = ReferenceLibrary::getInstance()->getInventoryModule(cat);
		int safety = 3;
		while (m_def->PreventDupe(2, item) && safety>0)
		{
			//reroll
			item = ReferenceLibrary::getInstance()->getInventoryModule(cat);
			safety--;
		}
		m_def->m_items[1][c] = item;

	}
	if (m_def->m_numrecruits > 0)
	{
		int c = 0;
		if (m_def->m_numrecruits > 1)
		{
			c = rand() % m_def->m_numrecruits;
		}

	}


}

int Colony::getDefence(int i)
{
	return m_defence[i];
}

int Colony::getDefenceBonus()
{
	int v = 0;
	for (int j = 0; j < 4; j++)
	{
		if (m_neighbours[j] != NULL)
		{
			v += m_neighbours[j]->getDefence(1);
		}
	}

	return v;
}