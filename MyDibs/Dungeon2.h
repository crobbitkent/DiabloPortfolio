#pragma once
#include <MyScene.h>
#include <MyGameSound.h>
class RandomMap;
class Player;
class Dungeon18;

class Dungeon2 : public MyScene
{
public:
	Dungeon2();
	~Dungeon2();

	void Loading() override;
	void SceneUpdate() override;

	void MakeMap();

	void Make3RoomsY();
	void Make3RoomsX();


	void SetLevel(int level);
	void SetDungeon18(Dungeon18* scene) { mDungeon18 = scene; }
	void ResetPos();
	Player* GetPlayer() { return mPlayer; }
	void SpawnMonsters();
	void Init();
private:
	void SetPlayer();


private:
	RandomMap* mMap1;
	RandomMap* mMap2;

	Player* mPlayer;

	Dungeon18* mDungeon18;
	MyGameSound::MyGameSoundStream* mBGM;
	// std::vector<Monster*> mMonsterPool;
};