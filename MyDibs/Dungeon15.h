#pragma once

#include <MyScene.h>
#include <MyGameSound.h>

#include "Values.h"

class Player;
class RandomMap;
class Dungeon2;
class Dungeon18;
class Monster;
class TestScene;

class Dungeon15 : public MyScene
{
public:
	Dungeon15();
	~Dungeon15();

	void Loading() override;
	void SceneUpdate() override;
	void Init();

	void MakeMap();

	void Make3RoomsY();
	void Make3RoomsX();


	void SetLevel(int level);
	void SetDungeon18(Dungeon18* scene) { mDungeon18 = scene; }
	void SetTestScene(TestScene* scene) { mTestScene = scene; }
	void ResetPosFromTest();
	void ResetPosFrom18();
	Player* GetPlayer() { return mPlayer; }
private:
	void SetPlayer();
	void SetMonster();
	void SpawnMonster();

private:
	RandomMap* mMap1;
	RandomMap* mMap2;

	Dungeon18* mDungeon18;
	TestScene* mTestScene;


	Player* mPlayer;

	// 실제 스폰 위치
	TileIndex mNewUpPos;
	TileIndex mNewDownPos;

	int mLevel = 0;

	MyGameSound::MyGameSoundStream* mBGM;

	// std::vector<Monster*> mMonsterPool;

};