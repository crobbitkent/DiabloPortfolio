#pragma once

#include <MyScene.h>
#include <MyGameSound.h>

#include "Values.h"

class Player;
class RandomMap;
class Dungeon2;
class Dungeon15;
class Monster;
class TestScene : public MyScene
{
public:
	TestScene();
	~TestScene();

	void Loading() override;
	void SceneUpdate() override;
	void Init();

	void MakeMap();

	void Make3RoomsY();
	void Make3RoomsX();


	void SetLevel(int level);
	void SetDungeon15(Dungeon15* scene) { mDungeon15 = scene; }
	void ResetPos();
	Player* GetPlayer() { return mPlayer; }
private:
	void SetPlayer();
	void SetMonster();
	void SpawnMonster();

private:
	RandomMap* mMap1;
	RandomMap* mMap2;

	// Dungeon2* mDungeon2;
	Dungeon15* mDungeon15;
	Player* mPlayer;

	// 실제 스폰 위치
	TileIndex mNewUpPos;
	TileIndex mNewDownPos;

	int mLevel = 0;

	MyGameSound::MyGameSoundStream* mBGM;

	// std::vector<Monster*> mMonsterPool;

};