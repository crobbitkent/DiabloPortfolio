#pragma once

#include <MyScene.h>
#include <MyGameSound.h>

#include "Values.h"

class Player;
class RandomMap;
class Dungeon2;
class Dungeon15;
class Monster;

class Dungeon18 : public MyScene
{
public:
	Dungeon18();
	~Dungeon18();

	void Loading() override;
	void SceneUpdate() override;
	void Init();

	void MakeMap();

	void Make3RoomsY();
	void Make3RoomsX();


	void SetLevel(int level);
	void SetDungeon2(Dungeon2* scene) { mDungeon2 = scene; }
	void SetDungeon15(Dungeon15* scene) { mDungeon15 = scene; }
	void ResetPosFrom15();
	void ResetPosFrom2();
	Player* GetPlayer() { return mPlayer; }
private:
	void SetPlayer();
	void SetMonster();
	void SpawnMonster();

private:
	RandomMap* mMap1;
	RandomMap* mMap2;

	Dungeon2* mDungeon2;
	Dungeon15* mDungeon15;
	Player* mPlayer;

	// 실제 스폰 위치
	TileIndex mNewUpPos;
	TileIndex mNewDownPos;

	int mLevel = 0;

	MyGameSound::MyGameSoundStream* mBGM;

	// std::vector<Monster*> mMonsterPool;

};