#pragma once

#include <MyMath.h>

class Item;
class ColliderMap;



class ItemManager
{
public:
	static ItemManager* Inst() 
	{
		if (nullptr == instance) 
		{ 
			instance = new ItemManager(); 
		} 

		return instance;
	}

	static void Init(ColliderMap* map);

	// static void AddItem(Item* item);
	static Item* RandomItem(int level);

	static void GenerateItem1(std::wstring name, int damage, int maxDamage, int armor, int maxArmor, int dura,
		CHAR_STATS require1, int require1Amount, CHAR_STATS require2, int require2Amount, 
		int price, int level, ITEM_TYPE type, int index);

	// static void GenerateItem2(std::wstring name, int price, int level, ITEM_TYPE type);

	// static Item* CloneItem(int index);
	 static Item* CloneItem(std::wstring name, int level);

	static Item* CloneAndDrop(int index, MyVector2 pos, int level);
	static Item* CloneAndDrop(std::wstring name, MyVector2 pos, int level);
	static void DropMoney(TileIndex index, int amount, int level);
	static void DropItem(TileIndex tileIndex, const wchar_t* name, int itemIndex, int level);

	static void SetCurColMap(ColliderMap* map) { mColMap = map; }
	static ColliderMap* GetColMap() { return mColMap; }

	static void SetLevel(int level) { mLevel = level; }
private:
	ItemManager();
	~ItemManager();


public:
protected:
private:
	static ItemManager* instance;
	
	static std::vector<Item*> mItemVec;
	static ColliderMap* mColMap;
	// static ColliderMap* mColMap2;
	// static std::map<>
	// static std::vector<Item*> mItem2;
	// static std::vector<Item*> mItem3;

	static int mLevel;
};

