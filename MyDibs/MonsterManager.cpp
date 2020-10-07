//
//#include "MonsterManager.h"
//#include "Monster.h"
//#include "GameManager.h"
//#include "Map.h"
//
//std::vector<Monster*> MonsterManager::mM1;
//std::vector<Monster*> MonsterManager::mM2;
//std::vector<Monster*> MonsterManager::mM3;
//int MonsterManager::index = 0;
//
//void MonsterManager::GenerateM1(int dlevel, const wchar_t* name, int level, int maxHp, int armor, int attack, int maxAttack, int toHit, int exp, wchar_t pixel)
//{
//	Monster* mon = new Monster();
//
//	mon->SetName(name);
//	mon->SetData(dlevel, level, maxHp, armor, attack, maxAttack, toHit, exp);
//
//	mon->Pixel(pixel);
//	
//	mM1.push_back(mon);
//
//	++index;
//}
//
//// 복사하여 위치 지정까지
//Monster* MonsterManager::CloneM1(int index, Position pos)
//{
//	if (0 > index || index >= mM1.size() || true == OverCheck(pos))
//	{
//		BOOM;
//
//		return nullptr;
//	}
//
//	Monster* temp = mM1[index]->CopyMonster(pos);
//
//	// Map::UnsetUnitAt(mPos);
//
//	temp->SetTile(pos);
//	temp->SetHP();
//
//	GameManager::GetMap()->SetUnitAt(pos, temp);
//
//	return temp;
//}
//
//void MonsterManager::Init()
//{
//	// 레벨 1
//	// Fallen ⒡
//	GenerateM1(1, L"Fallen One", 1, 2, 10, 1, 4, 15, 52, PIXEL_FALLEN);
//	// Scavenger ⒮
//	GenerateM1(2, L"Scavenger", 1, 3, 10, 1, 5, 20, 80, PIXEL_SCAVEN);
//	// Skeleton ㏜
//	GenerateM1(2, L"Skeleton", 1, 2, 0, 1, 5, 20, 64, PIXEL_SKEL);
//	// Zombie
//	GenerateM1(2, L"Zombie", 1, 1, 5, 2, 5, 10, 54, PIXEL_ZOMBIE);
//}
//
//MonsterManager::MonsterManager()
//{
//}
//
//
//MonsterManager::~MonsterManager()
//{
//
//}
//
//void MonsterManager::Clear()
//{
//	for (int y = 0; y < mM1.size(); y++)
//	{
//		if (nullptr != mM1[y])
//		{
//			delete mM1[y];
//			mM1[y] = nullptr;
//		}
//	}
//
//	for (int y = 0; y < mM2.size(); y++)
//	{
//		if (nullptr != mM2[y])
//		{
//			delete mM2[y];
//			mM2[y] = nullptr;
//		}
//	}
//
//	for (int y = 0; y < mM3.size(); y++)
//	{
//		if (nullptr != mM3[y])
//		{
//			delete mM3[y];
//			mM3[y] = nullptr;
//		}
//	}
//}
