#pragma once

#include <MyActor.h>
#include <MyGameSound.h>
#include "Values.h"

class ColliderMap;
class DibTile;
class RandomMap;
class TileColRenderer;

#ifdef RELEASEMODE
#ifdef X32
#pragma comment(lib, "MyBase_R32.lib")
#else X64
#pragma comment(lib, "MyBase_R64.lib")
#endif
#else DEBUGMODE
#ifdef X32
#pragma comment(lib, "MyBase_D32.lib")
#else X64
#pragma comment(lib, "MyBase_D64.lib")
#endif
#endif

// #pragma comment(lib, "MyBase_R64.lib")

class Monster :	public MyActor
{
public:
	static Monster* IsMonster(MyVector2 _Pos);

	void Update() override;
	void DebugRender() override;

	Monster* CopyMonster(MyVector2 pos);

	void SetCurPos(int indexX, int indexY);

	Monster(ColliderMap* colmap, RandomMap* randomMap);
	Monster(const Monster& other);
	virtual ~Monster();

	virtual void GetDamage(int damage) = 0;

	void CalIndex();

	TileIndex DibTileIndex() { return { mX , mY  }; }

	void DropMoney();
	void DropItem();
	bool SetDir(TileColRenderer * tile);
	bool SetDir(MyVector2 pos);
	bool SetDir(TileIndex index);

	UnitData GetUnitData() const { return mData; }
	int MonsterCount() { return mKillCount; }
protected:
	bool SetUnit();

	

	void CalDir(MyVector2 pos);

	void ResetPos(Monster* player);
	void ResetPosM(Monster* monster);

	void GoingToAttackM(Monster* player);

	void SetMove();
	void SetMoveM();

	void NormalDrop();

public:


protected:
	UnitData mData;
	int mCurHP;
	int mCurMP;

	MyVector2 mStart;
	bool mIsMoving;

	bool mIsAttacking;
	bool mAttacked;

	ColliderMap* mColMap;
	RandomMap* mRandomMap;

	MONSTER_DIR mDir;
	MONSTER_DIR mPrevDir;

	const wchar_t* mAllStateName[PS_MAX];
	const wchar_t* mAllDirName[MD_MAX];

	bool mIsDead = false;

	TileIndex mPrevTileIndex;
	DibTile* mDibTile;

	// 공격할 타일
	TileColRenderer* mDestDibTile;

	int mX;
	int mY;

	bool mIsDying;
	bool mIsChangingPath;
	bool mIsResting;

	float mRestingCount;
	float mMaxResting;

	bool mGoingToAttack;

	// PathFind
	std::list<MyVector2> mPathList;
	MyVector2 mCurDestination;

	float mAttackSpeed;
	float mMoveSpeed;

	int mVision;

	int mIncrease;
	int mBase;

	bool mIsDropped = false;

	MyGameSound::MyGameSoundStream* mSound;

	int mKillCount = 0;
private:






};

