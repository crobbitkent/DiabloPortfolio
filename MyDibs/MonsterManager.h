#pragma once

#include <vector>

class Monster;

class MonsterManager
{

public:
	static void Init();
	static void GenerateM1(
		int dlevel,
		const wchar_t* name,
		int level,
		int maxHp,
		int armor,
		int attack,
		int maxAttack,
		int toHit,
		int exp	,
		wchar_t pixel
	);
	
	static Monster* CloneM1(int index, MyVector2 pos);
	static void Clear();
private:
	MonsterManager();
	~MonsterManager();


private:
	// static std::vector<Monster*> mM1;
	// static std::vector<Monster*> mM2;
	// static std::vector<Monster*> mM3;
	// static int index;
};

