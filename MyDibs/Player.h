#pragma once

#include "Monster.h"

#include "Values.h"
#include <vector>
#include "DibTile.h"

class MyRenderer;
class ColliderMap;
class FogRenderer;
class DibRoom;
class StatRenderer;
class MyFile;
class Item;
class InvenBackRenderer;
class LevelRenderer;

class TestScene;
class Dungeon2;
class Dungeon15;
class Dungeon18;

class FontRenderer;
class YFontRenderer;

class Player : public Monster
{
	friend StatRenderer;
	friend FogRenderer;
	friend InvenBackRenderer;
	friend Item;
	friend ColliderMap;

	friend TestScene;
	friend Dungeon2;
	friend Dungeon15;
	friend Dungeon18;
public:
	Player* Inst() { return this; }

public:
	Player(ColliderMap* colmap, RandomMap* randomMap);
	virtual ~Player();
	void CopyPlayer(Player* player);

	void GetDamage(int damage) override;

	void SetCurPos(int indexX, int indexY);

	void SetCurrentState(PLAYER_STATE state, bool noneDir = false);
	void Update() override;
	void DebugRender() override;

	void SetDestination();

	const MONSTER_DIR PlayerDir() const { return mDir; }
	void PlayerDir(MONSTER_DIR dir) { mDir = dir; }

	bool CheckDir();
	void CheckDirChange();
	// void SetDir(TileColRenderer* dest);

	void WalkTask();

	void Idle();
	void Walk();
	void Attack();
	void Block();
	void Die();
	void RedSkill();
	void BlueSkill();
	void GetHit();

	void Fire();
	void StandingAttack(MyVector2 pos);

	//////////////////////////////////////// ColliderMap과 연동
	void SetColPosition(int indexX, int indexY);

	bool CheckWalkableTile();

	DibTile* GetDibTile() const { return mDibTile; }

	int DibTileX() const { return mDibTile->mX; }
	int DibTileY() const { return mDibTile->mY; }
	void GetExp(int exp) { mData.exp += exp; }

	TileIndex GetMouseIndex() { return mMouseIndex; }
	bool IsInvenOpen() { return mIsInvenOpen; }
	void ResetDir() { mDir = B; }
	void SetLevel(int level) { mLevel = level; }
	int GetLevel() { return mLevel; }
	bool IsStatOpen() { return mIsStatOpen; }
	///////////////////////////////////////// 아이템 폰트 관련
	void DescOn(std::wstring string1) { mIsShowing1 = true; mString1 = string1; }
	void DescOn(std::wstring string1, std::wstring string2) { mIsShowing2= true; mString1 = string1; mString2 = string2;	}
	void DescOn(std::wstring string1, std::wstring string2, std::wstring string3) { mIsShowing3 = true; mString1 = string1; mString2 = string2; mString3 = string3;}
	void DescYOn(std::wstring string1) { mIsYShowing1 = true; mString1 = string1; }
	void DescYOn(std::wstring string1, std::wstring string2) { mIsYShowing2 = true; mString1 = string1; mString2 = string2; }
	void DescYOn(std::wstring string1, std::wstring string2, std::wstring string3) { mIsYShowing3 = true; mString1 = string1; mString2 = string2; mString3 = string3; }


	void ShowDescription(std::wstring string1);
	void ShowDescription(std::wstring string1, std::wstring string2);
	void ShowDescription(std::wstring string1, std::wstring string2, std::wstring string3);
	void ShowYDescription(std::wstring string1);
	void ShowYDescription(std::wstring string1, std::wstring string2);
	void ShowYDescription(std::wstring string1, std::wstring string2, std::wstring string3);
	void UnshowDescription();
private:
	void SetAnimation();
	void AutoAnimation(const wchar_t* name, int index, float frameSpeed = 0.05f, bool isLoop = true);

	void GoingToMove(MyVector2 mousePos);
	void GoingToAttack(MyVector2 mousePos);
	void GoingToPick(MyVector2 mousePos);
	void PickItem(MyVector2 mousePos);
	void PickItem(TileColRenderer* tile);
	void PreMove();

	void CheckInRoom();
	bool CheckButton();
	bool CheckMouse();

	void OpenCloseInven();
	void OpenCloseStat();
	bool CheckMouse(MyVector2 mouse);
	void UpdateStat();
	void ResetPos(Monster* player);

	///////////////////////// 인벤토리
	int CheckAcquire(Item* item);
	void Acquire(Item* item, int base);
	void SuperAcquire(Item* item, int base);

	void ApplyItem(Item* data);
	void UnapplyItem(Item* data);
	///////////////////////// 경험치


	////////////////////// 스탯
	void CheckExp();
	
	void LevelUp();

	void LoadEXP();

	void SetMouseMonster();
private:
	MyAnimationRenderer* mAnimationRenderer;
	MyAnimationRenderer* mLevelUpRenderer;
	////////////////////// UI RENDERER /////////////////////////////////////
	MyRenderer* mUI;
	MyRenderer* mHP;
	MyRenderer* mMP;

	MyRenderer* mInven;
	MyRenderer* mStats;

	MyRenderer* mButton;
	MyRenderer* mPointButton;

	MyRenderer* mStrButton;
	MyRenderer* mMagButton;
	MyRenderer* mDexButton;
	MyRenderer* mVitButton;

	LevelRenderer* mLevelRender;

	Monster* mMonster;

	FogRenderer* mFog;
	InvenBackRenderer* mInvenBack;
	StatRenderer* mStatRenderer;

	FontRenderer* mFontRenderer;
	YFontRenderer* mYFontRenderer;
	// 플레이어 이동 관련

	PLAYER_STATE mState;

	bool mIsDirKey;

	int mDebugCount;

	bool mIsStandingAttack;

	MyVector2 mLastMouse;

	
	MyVector2 mNewDestination;

	DibRoom* mLastRoom;

	bool mIsInvenOpen;
	bool mIsStatOpen;
	bool mGoingToPick;

	int mButtonOffset;

	///////////////////////////////////////// STAT
	MyFile* mExpFile;
	std::vector<int> mExpVec;
	std::list<Item*> mItemList;
	int mBaseHP = 50;
	int mBaseMP = 20;

	int mItemAttack = 0;
	int mItemArmor = 0;
	int mItemToHit = 0;
	int mItemHP = 0;
	int mItemMP = 0;
	int mItemStr = 0;
	int mItemDex = 0;
	int mItemVit = 0;
	int mItemInt = 0;
	int mItemMag = 0;

	int mStr = 30;
	int mDex = 20;
	int mVit = 25;
	int mMag = 10;
	int mPoint = 0;

	int mCurStr = 30;
	int mCurDex = 20;
	int mCurVit = 25;
	int mCurMag = 10;

	bool mIsPoint;

	Item** mInventory;

	//std::vector<Item*> mInventory1;
	//std::vector<Item*> mInventory2;
	//std::vector<Item*> mInventory3;
	//std::vector<Item*> mInventory4;

	Item* mTileItem;
	TileIndex mMouseIndex;

	int mNextInvenIndex;

	/////////////////////////// 인벤토리 마우스
	bool mIsClicked;
	bool mIsItemOn;

	bool mHasWeapon = false;
	bool mHasHelm = false;
	bool mHasArmor = false;
	bool mHasShield = false;
	bool mHasLeftRing = false;
	bool mHasRightRing = false;
	bool mHasNeck = false;

	ItemData mItemData;

	Item* mWeapon = nullptr;
	Item* mArmor = nullptr;
	Item* mShield = nullptr;
	Item* mHelm = nullptr;
	Item* mLRing = nullptr;
	Item* mRRing = nullptr;
	Item* mNeck = nullptr;

	Item* mMouseItem = nullptr;

	int mLevel = 0;
	float mStepTime = 0.1f;
	int mStepCount = 0;
	int mStepMax = 4;

	// bool mAttackSound = false;
	bool mIsShowing1 = false;
	bool mIsShowing2 = false;
	bool mIsShowing3 = false;
	bool mIsYShowing1 = false;
	bool mIsYShowing2 = false;
	bool mIsYShowing3 = false;

	std::wstring mString1;
	std::wstring mString2;
	std::wstring mString3;

	Monster* mMouseMonster;

	std::wstring mTC;
	std::wstring mImune;
};


