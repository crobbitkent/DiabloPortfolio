#include "TestScene.h"

#include <MyInput.h>
#include <RandomNumber.h>
#include <MyWindowClass.h>
#include <MyTextureManager.h>
#include "Player.h"

#include "MapTester.h"
#include "RandomMap.h"
#include "ColliderMap.h"
#include "ItemManager.h"
#include "Item.h"
#include "Monster.h"
#include "DibRoom.h"

#include "Skeleton.h"
#include "Skeleton2.h"
#include "Butcher.h"
#include "Fallen.h"
#include "Fallen2.h"
#include "GoatMan.h"
#include "Scavenger.h"
#include "Zombie.h"
#include "Bat.h"


#include "Dungeon2.h"
#include "Dungeon15.h"

TestScene::TestScene()
{
}


TestScene::~TestScene()
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

void TestScene::Loading()
{
	mLevel = 1;

	MyScene::Loading();

	SortOnOff(LO_UPTILE);
	SortOnOff(LO_DOWNTILE);

	mMap1 = new RandomMap(this);

	MakeMap();

	mPlayer = new Player(mMap1->ColMap(), mMap1);
	mPlayer->SetLevel(mLevel);
	// SetLevel(1);

	SetPlayer();
	SetMonster();

	// Monster* sk = new Zombie(mMap1->ColMap(), mMap1, mPlayer, { 34,6 });

}

void TestScene::SceneUpdate()
{
	MyVector2 mouse = MyWindowClass::MainWindow()->MousePos();

	if (mMap1->ColMap() != ItemManager::Inst()->GetColMap())
	{
		ItemManager::Inst()->SetCurColMap(mMap1->ColMap());
	}


	if (mPlayer->GetTileIndex().X == mMap1->GetDownPos().X && mPlayer->GetTileIndex().Y == mMap1->GetDownPos().Y)
	{
		mBGM->Stop();
		SetCurScene(L"Dungeon15");
		// 넘어갈때 2층 미리 설정
		mDungeon15->ResetPosFromTest();
	}

	if (MyInput::Down(L"MOVET"))
	{
		
	}
	if (MyInput::Down(L"MOVEB"))
	{
		mPlayer->SetCurPos(mMap1->GetDownPos().X, mMap1->GetDownPos().Y + 1);
	}


	if (true == mMap1->GetIsChanging())
	{

	}
}

void TestScene::Init()
{
	mBGM = MyGameSound::SoundPlay(L"dlvla.mp3");
}

void TestScene::MakeMap()
{
	Make3RoomsY();

	// 중간방 가지치기
	mMap1->MakeRandomRooms({ 15, 14 }, { 10, 10 }, RT_LEFT, 3, 3, 3, 3);
	mMap1->ResetCount();
	mMap1->MakeRandomRooms({ 15, 14 }, { 10, 10 }, RT_RIGHT, 3, 3, 3, 3);
	mMap1->ResetCount();


	// 윗방 가지치기
	mMap1->MakeRandomRooms({ 15, 0 }, { 10, 10 }, RT_LEFT, 3, 3, 3, 3);
	mMap1->ResetCount();
	mMap1->MakeRandomRooms({ 15, 0 }, { 10, 10 }, RT_RIGHT, 3, 3, 3, 3);
	mMap1->ResetCount();



	// 막방 가지치기
	mMap1->MakeRandomRooms({ 15, 28 }, { 10, 10 }, RT_LEFT, 3, 3, 3, 3);
	mMap1->ResetCount();
	mMap1->MakeRandomRooms({ 15, 28 }, { 10, 10 }, RT_RIGHT, 3, 3, 3, 3);
	mMap1->ResetCount();

	// 마무리 계단
	if (false == mMap1->HasDownstair())
	{
		MyVector2 temp = MyVector2(14, 14);

		mMap1->MakeDownstair(temp);
	}


	// 마무리 (반드시 호출)
	if (false == mMap1->FinishMap())
	{
		MakeMap();

		mMap1->SetStartRoom();
		return;
	}


	/////////////////////////////////////////////////////////////////////

}

void TestScene::Make3RoomsY()
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

	//////////////////////////////////////////////////////////////////
	
}

void TestScene::Make3RoomsX()
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

	/////////////////////////////////////////////////////////////////////////

}

void TestScene::SetPlayer()
{
	ColliderMap* colMap = mMap1->ColMap();

	colMap->SetPlayer(mPlayer);
}

void TestScene::SetMonster()
{
	//#include "Skeleton.h"
//#include "Skeleton2.h"
//#include "Butcher.h"
//#include "Fallen.h"
//#include "Fallen2.h"
//#include "GoatMan.h"
//#include "Scavenger.h"
//#include "Zombie.h"
//#include "Bat.h"


	SpawnMonster();
}

void TestScene::SpawnMonster()
{
	int size = 0;
	int ran = 0;
	int x = 0;
	int y = 0;

	int roomSize = (mMap1->GetAllRoom()).size();

	for (size_t i = 0; i < roomSize; ++i)
	{
		if (true == mMap1->GetAllRoom()[i]->mIsStartRoom)
		{
			continue;
		}

		int temp = mMap1->GetAllRoom()[i]->Size.IntX();

		size = RandomNumber::GetRandomNumber(2, mMap1->GetAllRoom()[i]->Size.IntX());

		if (size > 8)
		{
			size = 8;
		}

		for (int j = 0; j < size; ++j)
		{
			ran = RandomNumber::GetRandomNumber(1, 5);

			x = mMap1->GetAllRoom()[i]->LeftTop().IntX() * 2  + ran + j ;
			y = mMap1->GetAllRoom()[i]->LeftTop().IntY() * 2  + ran + j ;

			Monster* sk = nullptr;

			switch (ran)
			{
			case 1:
				 sk = new Fallen(mMap1->ColMap(), mMap1, mPlayer, { x,y });
				break;

			case 2:
				sk = new Skeleton(mMap1->ColMap(), mMap1, mPlayer, { x,y });
				break;

			case 3:
				sk = new Zombie(mMap1->ColMap(), mMap1, mPlayer, { x,y });
				break;

			case 4:
				sk = new Scavenger(mMap1->ColMap(), mMap1, mPlayer, { x,y });
				break;

			case 5:
				sk = new Bat(mMap1->ColMap(), mMap1, mPlayer, { x,y });
				break;

			default:
				BOOM;
				break;
			}

		}
	}
}

void TestScene::ResetPos()
{
	// TestScene에서의 플레이어 정보를 받아온다.
	Player* tempP = mDungeon15->GetPlayer();

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
	mPlayer->SetCurPos(mMap1->GetDownPos().X, mMap1->GetDownPos().Y + 1);

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


