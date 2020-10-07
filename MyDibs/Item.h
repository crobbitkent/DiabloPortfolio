#pragma once

#include <MyMath.h>
#include <MyActor.h>
#include "Values.h"

class Player;

class TestScene;
class Dungeon2;
class Dungeon15;
class Dungeon18;

class Item : public MyActor
{
	friend class ItemManager;
	friend Player;

	friend TestScene;
	friend Dungeon2;
	friend Dungeon15;
	friend Dungeon18;
public:
	Item();
	Item(const Item& other);
	~Item();
	Item* CopyItem(int level);

	void SetTile(MyVector2 pos);
	void Update() override;
	void AllOff();
	void ResetPosition(int x, int y);
	void SetPosition(int x, int y);
	void ResetAni();
	void DebugRender() override;
private:
	void Unarm();

	bool CheckMouse();
	void SetInven(MyVector2 mousePos);
	void UnsetInven();
	int CheckInven(MyVector2 mousePos);
	void ArmWeapon();
	void ArmArmor();
	void ArmShield();
	void ArmHelm();
	void ArmLRing();
	void ArmRRing();
	void ArmNeck();

private:
	MyAnimationRenderer* mInGame;
	// MyAnimationRenderer* mSubInGame;

	// �ٴڿ� �� �������� �ٸ� RENDER_LEAGUE�� �ִϷ������� ���
	MyAnimationRenderer* mInGame2;
	MyAnimationRenderer* mInGameBack;

	MyRenderer* mInven;
	MyRenderer* mInvenEdge;

	ItemData mItemData;

	// �ִϸ��̼� Ȯ�ο�
	bool mIsOver = false;
	bool mIsTaken = false;

	Player* mPlayer;

	int mIndex;

	std::wstring mAniName;

	int mX;
	int mY;

	bool mIsOn = false;
	bool mIsSet = false;
	bool mWillBeOn = false;
	bool mWillBeOff = false;
	bool mWillBeSet = false;

	// ���⸦ �ٲٰ� �ִ� ���̴�.
	static bool mIsChanging;

	bool mIsEquiped = false;

	int mBase;

	bool mHasWeapon = false;
	bool mHasArmor = false;
	bool mHasHelm = false;
	bool mHasLeftRing = false;
	bool mHasRightRing = false;
	bool mHasNeck = false;
	bool mHasShield = false;

	Item* mMouseItem;

	int mLevel = 0;

	bool mIsDropped = false;
};

