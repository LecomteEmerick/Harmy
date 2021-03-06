#include "Army.h"

Army::Army()
{
	//currentID = 0;
	//this->unitList_ = std::vector<Unit*>();
	/*for (int i = 0; i < 10; i++)
	{
		this->unitList_.push_back(new Unit(currentID, 10,Point(0,0)));
		currentID++;
	}*/
}

Army::Army(std::string armyCode,int unitCount, int levelPerUnit)
{
	currentID = 0;
	this->armyCode_ = armyCode;
	Point currentSpawn;
	this->unitList_ = std::vector<Unit*>();
	for (int i = 0; i < unitCount; i++)
	{
		currentSpawn = Point(0,0);
		currentSpawn.x_set(currentSpawn.x_get() + (((std::rand() % 2) + 1) * std::rand() % this->rangeSpwan));
		currentSpawn.y_set(currentSpawn.y_get() + (((std::rand() % 2) + 1) * std::rand() % this->rangeSpwan));
		this->unitList_.push_back(new Unit(currentID, levelPerUnit,currentSpawn));
		currentID++;
	}
}

Army::Army(std::string armyCode, std::vector<Unit*> unitList)
{
	this->armyCode_ = armyCode;
	this->unitList_ = std::vector<Unit*>();
	for (int i = 0; i < unitList.size(); i++)
	{
		this->unitList_.push_back(new Unit((*unitList[i])));
	}
}

Army::Army(Army& army)
{
	this->armyCode_ = army.armyCode_;
	for (int i = 0; i < army.unitList_.size(); i++)
	{
		this->unitList_.push_back(new Unit(*army.unitList_[i]));
	}
	this->currentID = army.currentID;
}

Army& Army::operator=(const Army& army)
{
	if (this != &army)
	{
		this->armyCode_ = army.armyCode_;
		for (int i = 0; i < this->unitList_.size(); i++)
		{
			delete this->unitList_[i];
		}
		this->unitList_.clear();
		for (int i = 0; i < army.unitList_.size(); i++)
		{
			this->unitList_.push_back(new Unit(*army.unitList_[i]));
		}
		this->currentID = army.currentID;
	}
	return *this;
}

void Army::baseSpawn_set(Point value)
{
	for (int i = 0; i < this->unitList_.size(); i++)
	{
		Point currentSpawn = value;
		currentSpawn.x_set(currentSpawn.x_get() + (((std::rand() % 2) + 1) * std::rand() % this->rangeSpwan));
		currentSpawn.y_set(currentSpawn.y_get() + (((std::rand() % 2) + 1) * std::rand() % this->rangeSpwan));
		this->unitList_[i]->setPosition(currentSpawn.x_get(), currentSpawn.y_get());
	}
}

Unit& Army::unit_get(int uniqId)
{
	Unit* u = nullptr;
	for (int i = 0; i < this->unitList_.size(); i++)
	{
		if (this->unitList_[i]->UNIQ_ID_get() == uniqId)
			u = this->unitList_[i];
	}
	return *u;
}

Unit& Army::unitList_getAt(int index) const
{
	//if (uniqId < this->size_get())
	return *(this->unitList_[index]);
	//return nullptr;
}

Unit& Army::getNearestUnit(Point& pos)
{
	Unit* nearestUnit = &(unitList_getAt(0));
	int currentDist = pos.Distance(nearestUnit->position_get());
	for (int i = 1; i < this->size_get(); i++)
	{
		int dist = pos.Distance(unitList_getAt(i).position_get());
		if (dist < currentDist)
		{
			nearestUnit = &(unitList_getAt(i));
			currentDist = dist;
		}
	}
	return *nearestUnit;
}

Unit& Army::getFurtherUnit(Point& pos)
{
	Unit* furtherUnit = &(unitList_getAt(0));
	int currentDist = pos.Distance(furtherUnit->position_get());
	for (int i = 1; i < this->size_get(); i++)
	{
		int dist = pos.Distance(unitList_getAt(i).position_get());
		if (dist > currentDist)
		{
			furtherUnit = &(unitList_getAt(i));
			currentDist = dist;
		}
	}
	return *furtherUnit;
}

Unit& Army::getLowestUnit(int capacityId)
{
	Unit* lowestUnit = &(unitList_getAt(0));
	float currentValue = lowestUnit->capacity_get(capacityId)->value_get();
	for (int i = 1; i < this->size_get(); i++)
	{
		float value = unitList_getAt(i).capacity_get(capacityId)->value_get();
		if (value < currentValue)
		{
			lowestUnit = &(unitList_getAt(i));
			currentValue = value;
		}
	}
	return *lowestUnit;
}

Unit& Army::getHiggestUnit(int capacityId)
{
	Unit* higgestUnit = &(unitList_getAt(0));
	float currentValue = higgestUnit->capacity_get(capacityId)->value_get();
	for (int i = 1; i < this->size_get(); i++)
	{
		float value = unitList_getAt(i).capacity_get(capacityId)->value_get();
		if (value > currentValue)
		{
			higgestUnit = &(unitList_getAt(i));
			currentValue = value;
		}
	}
	return *higgestUnit;
}

void Army::purge()
{
	int number = 0;
	std::vector<int> removedList = std::vector<int>();
	for (int i = 0; i < this->size_get(); i++)
	{
		Unit& currentUnit = this->unitList_getAt(i);
		if (!currentUnit.isAlive())
			removedList.push_back(number);
		else
			number++;
	}
	for (int i = 0; i < removedList.size(); i++)
	{
		this->unitList_.erase(this->unitList_.begin() + removedList[i]);
	}
}

Army Army::mutate()
{
	Army cpyArmy = Army(*this);
	int armyLength = cpyArmy.unitList_.size();
	int mutationType = std::rand() % 2;
	if (mutationType == 0)
	{
		for (int i = 0; i < cpyArmy.unitList_.size(); i++)
		{
			cpyArmy.unitList_[i] = new Unit(i, cpyArmy.unitList_[i]->level_get(), cpyArmy.unitList_[i]->position_get());
		}
	}
	else{
		int startEnsemble =0;
		int endEnsemble =0;
		while (startEnsemble == endEnsemble)
		{
			startEnsemble = std::rand() % armyLength;
			endEnsemble = std::rand() % armyLength+1;
		}
		if (endEnsemble < startEnsemble){
			int tmp = startEnsemble;
			startEnsemble = endEnsemble;
			endEnsemble = tmp;
		}
		for (int i = startEnsemble; i < endEnsemble; i++)
		{
			cpyArmy.unitList_[i] = new Unit(cpyArmy.unitList_[i]->mutate());
		}
	}
	return cpyArmy;
}

Army& Army::operator*(const Army& army)
{
	Army newArmy = Army();
	int minSize = this->size_get() < army.size_get() ? this->size_get() : army.size_get();
	int numberUnitMutation = std::rand() % minSize;
	for (int i = 0; i < numberUnitMutation; i++)
	{
		newArmy.unitList_.push_back(new Unit(this->unitList_getAt(i) * army.unitList_getAt(i)));
	}
	int takemyArmybefore = std::rand() % 2;
	if (takemyArmybefore == 1)
	{
		int restUnit = minSize - numberUnitMutation;
		int random = std::rand() % restUnit;
		for (int i = 0; i < random; i++)
		{
			newArmy.unitList_.push_back(this->unitList_[numberUnitMutation + i]);
		}
		for (int i = numberUnitMutation + random; i < army.size_get(); i++)
		{
			newArmy.unitList_.push_back(army.unitList_[i]);
		}
	}
	else{
		int restUnit = minSize - numberUnitMutation;
		int random = std::rand() % restUnit;
		for (int i = 0; i < random; i++)
		{
			newArmy.unitList_.push_back(army.unitList_[numberUnitMutation + i]);
		}
		for (int i = numberUnitMutation + random; i < this->size_get(); i++)
		{
			newArmy.unitList_.push_back(this->unitList_[i]);
		}
	}
	return newArmy;
}

void Army::save(int size, int level) {
	char fileName[50];
	sprintf_s(fileName, 50, "army_%d_%d.save", size, level);
	std::ofstream file(fileName, std::ios::out | std::ios::trunc);  // ouverture en �criture avec effacement du fichier ouvert
	if (file)
	{
		for each (Unit* unit in unitList_)
		{
			unit->save(file);
		}
		file.close();
	}
}

Army::~Army()
{
	for (int i = 0; i < this->unitList_.size(); i++)
	{
		delete this->unitList_[i];
	}
	this->unitList_.clear();
}
