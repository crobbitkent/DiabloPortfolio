#pragma once

#include <MyMath.h>

class Map;
class Player;

class GameManager
{

public:
	static MyVector2 ScreenSize() { return mScreenSize;	}

	static void Init(MyVector2 _Size);
	static void Clear();
	static void Print(int _Frame);
	static void SetPixel(MyVector2 _Pos, wchar_t _Pixel);
	
	// static Map* GetMap() { return mMap; }
	// static Player* GetPlayer() { return mPlayer; }

	static void SetPlayer(Player* player);
	static void SetMap(Map* map);
	//static MyArr<MyArr<class Object*>>* GetAll() 
	//{ 
	//	return &mAllObject; 
	//}

	//static class Object* GetGameObject(int _X, int _Y) 
	//{
	//	return mAllObject[_Y][_X];
	//}
	static void PrintPlayerStatus();
	static void PrintMonster();
private:
	GameManager();
	~GameManager();

	static void DeleteScreen();
	static void CreateWall();


private:
	static MyVector2 mScreenSize;



	// static wchar_t m_BaseChar;
	// static class Player* mPlayer;
	// static Map* mMap;
};

