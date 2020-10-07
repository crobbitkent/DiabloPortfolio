//#include "StageManager.h"
//#include "GameManager.h"
//
//#include "Map.h"
//
//std::vector<Map*> StageManager::mAllMap = std::vector<Map*>();
//int StageManager::currentIndex = 0;
//
//void StageManager::ClearMaps()
//{
//	for (int i = 0; i < mAllMap.capacity(); i++)
//	{
//		if (nullptr != mAllMap[i])
//		{
//			delete mAllMap[i];
//			mAllMap[i] = nullptr;
//		}
//	}
//}
//
//StageManager::StageManager()
//{
//}
//
//
//StageManager::~StageManager()
//{
//}
//
//void StageManager::AddMap(Map* map)
//{
//	mAllMap.push_back(map);
//
//	//for (int y = 0; y < GameManager::ScreenSize().Y; y++)
//	//{
//	//	for (int x = 0; x < GameManager::ScreenSize().X; x++)
//	//	{
//	//		mAllTile[y][x].FindNeighbor();
//	//	}
//	//}
//}
//
//
//Map* StageManager::GoToNextLevel()
//{
//	++currentIndex;
//
//	if (mAllMap.size() <= currentIndex)
//	{
//		return nullptr;
//	}
//
//	return mAllMap[currentIndex];
//}
//
//Map* StageManager::GoToPrevLevel()
//{
//	--currentIndex;
//
//	if (0 > currentIndex)
//	{
//		return nullptr;
//	}
//
//
//
//	return mAllMap[currentIndex];
//}