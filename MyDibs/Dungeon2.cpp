#include "Dungeon2.h"

#include <MyInput.h>
#include <RandomNumber.h>
#include <MyWindowClass.h>
#include <MyTextureManager.h>
#include "ItemManager.h"
#include "Item.h"
#include "Player.h"

#include "MapTester.h"
#include "RandomMap.h"
#include "ColliderMap.h"

#include "Monster.h"
#include "Skeleton.h"
#include "Dungeon18.h"

Dungeon2::Dungeon2()
{
}


Dungeon2::~Dungeon2()
{
	if (nullptr != mMap1)
	{
		delete mMap1;
		mMap1 = nullptr;
	}

	if (nullptr != mMap2)
	{
		delete mMap2;
		mMap2 = nullptr;
	}
}

void Dungeon2::Loading()
{
	mLevel = 4;

	MyScene::Loading();

	SortOnOff(LO_UPTILE);
	SortOnOff(LO_DOWNTILE);

	mMap1 = new RandomMap(this);

	MakeMap();

	mPlayer = new Player(mMap1->ColMap(), mMap1);
	mPlayer->SetLevel(mLevel);

	SetPlayer();

}

void Dungeon2::SceneUpdate()
{
	MyVector2 mouse = MyWindowClass::MainWindow()->MousePos();

	if (mMap1->ColMap() != ItemManager::Inst()->GetColMap())
	{
		ItemManager::Inst()->SetCurColMap(mMap1->ColMap());
	}

	if (MyInput::Down(L"MOVEL"))
	{
		SetCurScene(L"TestScene");
		
	}
	if (MyInput::Down(L"MOVER"))
	{
		
	}
	if (MyInput::Down(L"MOVET"))
	{
		mPlayer->SetCurPos(mMap1->GetBPos().X, mMap1->GetBPos().Y);
	}
	if (MyInput::Down(L"MOVEB"))
	{
		ResetPos();
	}

	if (mPlayer->GetTileIndex().X == mMap1->GetStartPos1().X && mPlayer->GetTileIndex().Y == mMap1->GetStartPos1().Y)
	{
		mBGM->Stop();
		SetCurScene(L"Dungeon18");
		mDungeon18->ResetPosFrom2();
	}
	else if(mPlayer->GetTileIndex().X == mMap1->GetStartPos2().X && mPlayer->GetTileIndex().Y == mMap1->GetStartPos2().Y)
	{
		mBGM->Stop();
		SetCurScene(L"Dungeon18");
		mDungeon18->ResetPosFrom2();
	}
}

void Dungeon2::MakeMap()
{
	Make3RoomsY();


	// 윗방 가지치기
	mMap1->MakeRandomBRooms({ 15, 0 }, { 10, 10 }, RT_LEFT, 3, 3, 3, 3);
	mMap1->ResetCount();

	if (true == mMap1->HasBRoom())
	{
		mMap1->MakeRandomRooms({ 15, 0 }, { 10, 10 }, RT_RIGHT, 3, 3, 3, 3);
		mMap1->ResetCount();
	}
	else
	{
		mMap1->MakeRandomBRooms({ 15, 0 }, { 10, 10 }, RT_RIGHT, 3, 3, 3, 3);
		mMap1->ResetCount();
	}

	if (true == mMap1->HasBRoom())
	{
		// 중간방 가지치기
		mMap1->MakeRandomRooms({ 15, 14 }, { 10, 10 }, RT_LEFT, 3, 3, 3, 3);
		mMap1->ResetCount();
	}
	else
	{
		// 중간방 가지치기
		mMap1->MakeRandomBRooms({ 15, 14 }, { 10, 10 }, RT_LEFT, 3, 3, 3, 3);
		mMap1->ResetCount();
	}

	if (true == mMap1->HasBRoom())
	{
		mMap1->MakeRandomRooms({ 15, 14 }, { 10, 10 }, RT_RIGHT, 3, 3, 3, 3);
		mMap1->ResetCount();
	}
	else
	{
		mMap1->MakeRandomBRooms({ 15, 14 }, { 10, 10 }, RT_RIGHT, 3, 3, 3, 3);
		mMap1->ResetCount();
	}


	if (true == mMap1->HasBRoom())
	{
		// 막방 가지치기
		mMap1->MakeRandomRooms({ 15, 28 }, { 10, 10 }, RT_LEFT, 3, 3, 3, 3);
		mMap1->ResetCount();
	}
	else
	{
		// 막방 가지치기
		mMap1->MakeRandomBRooms({ 15, 28 }, { 10, 10 }, RT_LEFT, 3, 3, 3, 3);
		mMap1->ResetCount();
	}

	if (true == mMap1->HasBRoom())
	{
		mMap1->MakeRandomRooms({ 15, 28 }, { 10, 10 }, RT_RIGHT, 3, 3, 3, 3);
		mMap1->ResetCount();
	}
	else
	{
		mMap1->MakeRandomBRooms({ 15, 28 }, { 10, 10 }, RT_RIGHT, 3, 3, 3, 3);
		mMap1->ResetCount();
	}

	if (false == mMap1->HasBRoom())
	{
		mMap1->SetBRoom();
	}

	if (false == mMap1->HasBRoom())
	{
		BOOM;
	}

	// 마무리 (반드시 호출)
	mMap1->FinishMap();

	// 마무리 계단
	if (false == mMap1->HasDownstair())
	{
		MyVector2 temp = MyVector2(15, 14);

		mMap1->MakeDownstair(temp);
	}
}

void Dungeon2::Make3RoomsY()
{
	// 상
	mMap1->MakeARoom({ 15, 0 }, { 10, 10 }, { 0, 0 }, { 0, 0 }, RT_MAX);
	mMap1->MakeTopEleven({ 17, 11 }, { 6, 2 }, { 15, 0 }, { 10, 10 });
	// 중
	mMap1->MakeARoom({ 15, 14 }, { 10, 10 }, { 0, 0 }, { 0, 0 }, RT_MAX);
	mMap1->MakeAConnection({ 15, 14 }, { 10, 10 }, { 17, 11 }, { 6, 2 }, RT_BOTTOM);
	mMap1->MakeTopEleven({ 17, 25 }, { 6, 2 }, { 15, 14 }, { 10, 10 });
	// 하
	mMap1->MakeARoom({ 15, 28 }, { 10, 10 }, { 0, 0 }, { 0, 0 }, RT_MAX);
	mMap1->MakeAConnection({ 15, 28 }, { 10, 10 }, { 17, 25 }, { 6, 2 }, RT_BOTTOM);

	mMap1->MakeArchY({ 17, 11 }, { 6, 2 }, { 15, 0 }, { 10, 10 });

	mMap1->MakeArchY({ 17, 25 }, { 6, 2 }, { 15, 14 }, { 10, 10 });

	mMap1->SetColumnAt(19, 4);
	mMap1->SetColumnAt(22, 4);

	mMap1->SetColumnAt(19, 7);
	mMap1->SetColumnAt(22, 7);

	mMap1->SetColumnAt(19, 18);
	mMap1->SetColumnAt(22, 18);

	mMap1->SetColumnAt(19, 21);
	mMap1->SetColumnAt(22, 21);

	mMap1->SetColumnAt(19, 32);
	mMap1->SetColumnAt(22, 32);

	mMap1->SetColumnAt(19, 35);
	mMap1->SetColumnAt(22, 35);

	
}

void Dungeon2::Make3RoomsX()
{
	mMap1->MakeARoom({ 0, 14 }, { 10, 10 }, { 0, 0 }, { 0, 0 }, RT_MAX);
	mMap1->MakeARoom({ 11, 16 }, { 2, 6 }, { 0, 0 }, { 0, 0 }, RT_MAX);

	mMap1->MakeARoom({ 14, 14 }, { 10, 10 }, { 0, 0 }, { 0, 0 }, RT_MAX);
	mMap1->MakeARoom({ 25, 16 }, { 2, 6 }, { 0, 0 }, { 0, 0 }, RT_MAX);

	mMap1->MakeARoom({ 28, 14 }, { 10, 10 }, { 0, 0 }, { 0, 0 }, RT_MAX);

	mMap1->SetColumnAt(4, 18);
	mMap1->SetColumnAt(8, 18);

	mMap1->SetColumnAt(4, 21);
	mMap1->SetColumnAt(8, 21);

	mMap1->SetColumnAt(18, 18);
	mMap1->SetColumnAt(21, 18);

	mMap1->SetColumnAt(18, 21);
	mMap1->SetColumnAt(21, 21);

	mMap1->SetColumnAt(32, 18);
	mMap1->SetColumnAt(35, 18);

	mMap1->SetColumnAt(32, 21);
	mMap1->SetColumnAt(35, 21);

	
}

void Dungeon2::SetPlayer()
{
	ColliderMap* colMap = mMap1->ColMap();

	colMap->SetPlayer(mPlayer);
}

void Dungeon2::SetLevel(int level)
{
}

// 넘어올때 1층에서 호출한다.
void Dungeon2::ResetPos()
{

	// TestScene에서의 플레이어 정보를 받아온다.
	Player* tempP = mDungeon18->GetPlayer();

	mPlayer->CopyPlayer(tempP);

	std::list<Item*>::iterator iter = tempP->mItemList.begin();
	std::list<Item*>::iterator iterEnd = tempP->mItemList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (nullptr != *iter)
		{
			Item* tempI = new Item(*(*iter));

			tempI->mPlayer = mPlayer;

			mPlayer->SuperAcquire(tempI, tempI->mBase);
			// tempI->SetInven(tempI->mInven->SubPos());
		}
	}

	mPlayer->ResetDir();
	mPlayer->SetCurPos(mMap1->GetStartPos1().X, mMap1->GetStartPos1().Y + 1);

	ItemManager::Inst()->SetCurColMap(mMap1->ColMap());

	//////////////////////////////////////////////////////////

	if (nullptr != tempP->mWeapon)
	{
		Item* tempI = new Item(*tempP->mWeapon);
		tempI->mPlayer = mPlayer;

		// mPlayer->mWeapon = tempI;

		tempI->mIsTaken = true;
	//	tempI->On();
		tempI->ArmWeapon();

	}
	
	
	if (nullptr != tempP->mArmor)
	{
		Item* tempI = new Item(*tempP->mArmor);
		tempI->mPlayer = mPlayer;

		mPlayer->mArmor = tempI;

		tempI->ArmArmor();		
		tempI->On();
	}

	if (nullptr != tempP->mShield)
	{
		Item* tempI = new Item(*tempP->mShield);
		tempI->mPlayer = mPlayer;

		mPlayer->mShield = tempI;

		tempI->ArmShield();
	}
	
	if (nullptr != tempP->mHelm)
	{
		Item* tempI = new Item(*tempP->mHelm);
		tempI->mPlayer = mPlayer;

		mPlayer->mHelm = tempI;

		tempI->ArmHelm();
	}

	if (nullptr != tempP->mLRing)
	{
		Item* tempI = new Item(*tempP->mLRing);
		tempI->mPlayer = mPlayer;

		mPlayer->mLRing = tempI;

		tempI->ArmLRing();
	}

	if (nullptr != tempP->mRRing)
	{
		Item* tempI = new Item(*tempP->mRRing);
		tempI->mPlayer = mPlayer;

		mPlayer->mRRing = tempI;

		tempI->ArmRRing();
	}

	if (nullptr != tempP->mNeck)
	{
		Item* tempI = new Item(*tempP->mNeck);
		tempI->mPlayer = mPlayer;

		mPlayer->mNeck = tempI;

		tempI->ArmNeck();
	}
}

void Dungeon2::SpawnMonsters()
{
	int size = 0;

	//for (size_t i = 0; i < mMap1->GetAllRoom().size; ++i)
	//{
	//	size = RandomNumber::GetRandomNumber(2, mMap1->GetAllRoom()[i]->Size.IntX());


	//}
}

void Dungeon2::Init()
{
	mBGM = MyGameSound::SoundPlay(L"dlvld.mp3");
}
