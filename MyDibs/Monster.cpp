#include "Monster.h"

#include <MyScene.h>
#include <MyInput.h>
#include <MyTime.h>
#include <MyAnimationRenderer.h>
#include <MyCollider.h>
#include <MyWindowClass.h>
#include <RandomNumber.h>

#include "ColliderMap.h"
#include "RandomMap.h"
#include "TileColRenderer.h"
#include "ItemManager.h"

// #pragma comment(lib, "MyBase.lib")

// #pragma comment(lib, "MyBase_D64.lib")

//#ifdef RELEASEMODE
//#ifdef X32
//#pragma comment(lib, "MyBase_R32.lib")
//#else X64
//#pragma comment(lib, "MyBase_R64.lib")
//#endif
//#else DEBUGMODE
//#ifdef X32
//#pragma comment(lib, "MyBase_D32.lib")
//#else X64
//#pragma comment(lib, "MyBase_D64.lib")
//#endif
//#endif


Monster::Monster(ColliderMap* colmap, RandomMap* randomMap)
	: mColMap(colmap)
	, mRandomMap(randomMap)
	, mIsDying(false)
	, mIsChangingPath(false)
	, mMoveSpeed(3.f)
	, mCurDestination(MyVector2::ZERO)
	, mDestDibTile(nullptr)
	, mStart(MyVector2::ZERO)
	, mIsMoving(false)
	, mIsAttacking(false)
	, mAttacked(false)
	, mIsResting(false)
	, mGoingToAttack(false)
	, mVision(0)
{

}

Monster::Monster(const Monster & other)
{

}


Monster::~Monster()
{



}

// 자신의 위치에 따라서 Index를 계산한다.
void Monster::CalIndex()
{
	MyVector2 pos = CalPos();

	TileColRenderer* temp = mColMap->GetTile(pos.X, pos.Y);

	if (nullptr == temp)
	{
		return;
	}


	mTileIndex = temp->GetTileIndex();

	MyVector2 tempDib = mRandomMap->SubToDib(mTileIndex.X, mTileIndex.Y);

	mDibTile = mRandomMap->mAllTile[tempDib.IntY()][tempDib.IntX()];
}

void Monster::Update()
{
}

void Monster::DebugRender()
{
}

void Monster::SetCurPos(int indexX, int indexY)
{
	// 만들 곳을 못 찾으면 삭제

	TileIndex temp = mColMap->SetActorIndex(indexX, indexY, this);

	if (TileIndex({ -1,-1 }) == temp)
	{
		KillThis();

		return;
	}

	mTileIndex.X = temp.X;
	mTileIndex.Y = temp.Y;

	mPrevTileIndex = mTileIndex;

	float x = (mTileIndex.X * QUARTER_TILE_WIDTH) + (mTileIndex.Y * -QUARTER_TILE_WIDTH);
	float y = (mTileIndex.X * QUARTER_TILE_HEIGHT) + (mTileIndex.Y * QUARTER_TILE_HEIGHT) - TILE_SUBOFFSETY / 2;

	Pos({ x, y });
	mStart = Pos();
	// Scene()->CamMove({ x, y });
}


bool Monster::SetUnit()
{
	// 플레이어의 현재 위치에 따른 Index 얻어내기

	if (true == mIsDead)
	{
		return false;
	}

	CalIndex();

	if (mPrevTileIndex.Index != mTileIndex.Index)
	{
		if (mTileIndex.X <= 0)
		{
			mTileIndex.X = 1;
		}
		if (mTileIndex.Y <= 0)
		{
			mTileIndex.Y = 1;
		}

		// 지도의 타일에 플레이어를 설정, 해제 해준다. 
		mColMap->UpdateActorIndex(mTileIndex.X, mTileIndex.Y, mPrevTileIndex.X, mPrevTileIndex.Y, this);
		mPrevTileIndex = mTileIndex;

	}

	SetTileIndex(mTileIndex);

	mX = mDibTile->mX;
	mY = mDibTile->mY;

	//mRandomMap->mAllTile[mY + 1][mX]->mWallRenderer->SetBF(20);
	//// mRandomMap->mAllTile[mY][mX + 1]->mWallRenderer->SetBF(20);
	//// mRandomMap->mAllTile[mY - 1][mX]->mWallRenderer->SetBF(20);
	//// mRandomMap->mAllTile[mY][mX - 1]->mWallRenderer->SetBF(20);
	//mRandomMap->mAllTile[mY + 1][mX + 1]->mWallRenderer->SetBF(20);
	//// mRandomMap->mAllTile[mY - 1][mX - 1]->mWallRenderer->SetBF(20);
	//// mRandomMap->mAllTile[mY - 1][mX + 1]->mWallRenderer->SetBF(20);
	//mRandomMap->mAllTile[mY + 1][mX - 1]->mWallRenderer->SetBF(20);

	return true;
}


bool Monster::SetDir(TileIndex index)
{
	TileIndex test = index;

	MyVector2 dest;

	dest.X = (test.X * QUARTER_TILE_WIDTH) + (test.Y * -QUARTER_TILE_WIDTH);
	dest.Y = (test.X * QUARTER_TILE_HEIGHT) + (test.Y * QUARTER_TILE_HEIGHT) - TILE_SUBOFFSETY / 2;

	float x = Pos().X - dest.X;
	float y = Pos().Y - dest.Y;

	float temp = sqrtf(x * x + y * y);

	float newX = x / temp;
	float newY = y / temp;

	MyVector2 newDir = MyVector2(newX, newY);

	if (MyVector2(-1.f, 0.f) == newDir)
	{
		mDir = RT;
	}
	else if (MyVector2(1.f, 0.f) == newDir)
	{
		mDir = LB;
	}
	else if (MyVector2(0.f, -1.f) == newDir)
	{
		mDir = RB;
	}
	else if (MyVector2(0.f, 1.f) == newDir)
	{
		mDir = LT;
	}
	else if (MyVector2(0.894427180f, 0.447213590f) == newDir)
	{
		mDir = L;
	}
	else if (MyVector2(-0.894427180f, 0.447213590f) == newDir)
	{
		mDir = T;
	}
	else if (MyVector2(-0.894427180f, -0.447213590f) == newDir)
	{
		mDir = R;
	}
	else if (MyVector2(0.894427180f, -0.447213590f) == newDir)
	{
		mDir = B;
	}
	else
	{

	}

	return true;
}

bool Monster::SetDir(TileColRenderer * tile)
{
	if (nullptr == tile)
	{
		return false;
	}

	TileIndex test = tile->GetTileIndex();

	MyVector2 dest;

	dest.X = (tile->GetTileIndex().X * QUARTER_TILE_WIDTH) + (tile->GetTileIndex().Y * -QUARTER_TILE_WIDTH);
	dest.Y = (tile->GetTileIndex().X * QUARTER_TILE_HEIGHT) + (tile->GetTileIndex().Y * QUARTER_TILE_HEIGHT) - TILE_SUBOFFSETY / 2;

	float x = Pos().X - dest.X;
	float y = Pos().Y - dest.Y;

	float temp = sqrtf(x * x + y * y);

	float newX = x / temp;
	float newY = y / temp;

	MyVector2 newDir = MyVector2(newX, newY);

	if (MyVector2(-1.f, 0.f) == newDir)
	{
		mDir = RT;
	}
	else if (MyVector2(1.f, 0.f) == newDir)
	{
		mDir = LB;
	}
	else if (MyVector2(0.f, -1.f) == newDir)
	{
		mDir = RB;
	}
	else if (MyVector2(0.f, 1.f) == newDir)
	{
		mDir = LT;
	}
	else if (MyVector2(0.894427180f, 0.447213590f) == newDir)
	{
		mDir = L;
	}
	else if (MyVector2(-0.894427180f, 0.447213590f) == newDir)
	{
		mDir = T;
	}
	else if (MyVector2(-0.894427180f, -0.447213590f) == newDir)
	{
		mDir = R;
	}
	else if (MyVector2(0.894427180f, -0.447213590f) == newDir)
	{
		mDir = B;
	}
	else
	{

	}

	return true;
}

bool Monster::SetDir(MyVector2 pos)
{
	mIsAttacking = false;

	float x = Pos().X - pos.X;
	float y = Pos().Y - pos.Y;

	float temp = sqrtf(x * x + y * y);

	float newX = x / temp;
	float newY = y / temp;

	MyVector2 newDir = MyVector2(newX, newY);

	if (MyVector2(-1.f, 0.f) == newDir)
	{
		mDir = RT;
	}
	else if (MyVector2(1.f, 0.f) == newDir)
	{
		mDir = LB;
	}
	else if (MyVector2(0.f, -1.f) == newDir)
	{
		mDir = RB;
	}
	else if (MyVector2(0.f, 1.f) == newDir)
	{
		mDir = LT;
	}
	else if (MyVector2(0.894427180f, 0.447213590f) == newDir)
	{
		mDir = L;
	}
	else if (MyVector2(-0.894427180f, 0.447213590f) == newDir)
	{
		mDir = T;
	}
	else if (MyVector2(-0.894427180f, -0.447213590f) == newDir)
	{
		mDir = R;
	}
	else if (MyVector2(0.894427180f, -0.447213590f) == newDir)
	{
		mDir = B;
	}
	else
	{
		mDir = RB;
		Pos(mStart);
		//////////////////////////////////////// 버그지만 자연스럽게 넘기자
	}

	return true;
}

void Monster::CalDir(MyVector2 pos)
{
	mIsAttacking = false;

	MyVector2 newPos = { pos.X - 32, pos.Y - 16 };

	MyVector2 tempPos = CalPos() - MyVector2(32.f , 10.f );

	float x = tempPos.X - newPos.X;
	float y = tempPos.Y - newPos.Y;

	float temp = sqrtf(x * x + y * y);

	float newX = x / temp;
	float newY = y / temp;

	MyVector2 newDir = MyVector2(-newX, -newY);

	MyVector2 newOffset = { newDir.X * 40, newDir.Y * 32 };
	MyVector2 test = newOffset;

	//////////////////////////////////////////////////////////////////////////////
	int intX = (int)std::roundf((test.X / QUARTER_TILE_WIDTH + test.Y / QUARTER_TILE_HEIGHT) / 2);
	int intY = (int)std::roundf((test.Y / QUARTER_TILE_HEIGHT + -(test.X / QUARTER_TILE_WIDTH)) / 2);

	TileIndex returnIndex;
	returnIndex.X = intX;
	returnIndex.Y = intY;
	//////////////////////////////////////////////////////////////////////////////

	int testX = (int)roundf(-newX);
	int testY = (int)roundf(-newY);

	TileIndex tempIndex = GetTileIndex() + returnIndex;
	TileColRenderer* tempTile = mColMap->FindTile(tempIndex.X, tempIndex.Y);

	MyVector2 dirPos = mColMap->IndexToPosM(tempIndex);

	SetDir(dirPos);
}



void Monster::ResetPosM(Monster* monster)
{
	Pos(mStart);

	TileIndex cur = GetTileIndex();

	for (size_t i = 0; i < 8; ++i)
	{
		TileIndex temp = cur + ColliderMap::mDirArr[i];

		if (monster == mColMap->GetTile(temp.X, temp.Y)->GetActor())
		{
			mColMap->GetTile(temp.X, temp.Y)->UnsetActor();
		}
	}

	mPathList.clear();
}


void Monster::GoingToAttackM(Monster* player)
{
	MyVector2 playerPos = player->CalPos();
	TileIndex playerIndex = player->GetTileIndex();
	TileColRenderer* destTile = mColMap->GetTile(playerIndex.X, playerIndex.Y);

	// 클릭한 타일에 몬스터가 있다면!
	// 주변 타일 중 가장 가까운 타일로 간다.
	if (nullptr != destTile && nullptr != destTile->GetActor())
	{
		// mPathList = mColMap->PathFindAttack(CalPos(), playerPos);

		if (0 != mPathList.size())
		{
			mDestDibTile = mColMap->GetTile(playerIndex.X, playerIndex.Y);

			mPathList.pop_back();

			// 미리 한칸을 움직여서 겹치는 것을 방지!
			MyVector2 first = mCurDestination;
			first -= CamPos();
			TileColRenderer* firstTile = mColMap->GetTile(first.X, first.Y);
			firstTile->SetActor(this);

			mIsChangingPath = false;

			mGoingToAttack = true;
		}
	}
	else
	{
		mPathList = mColMap->PathFind(CalPos(), playerPos);

		if (0 != mPathList.size())
		{
			mPathList.pop_front();

			// 미리 한칸을 움직여서 겹치는 것을 방지!
			MyVector2 first = mCurDestination;
			first -= CamPos();
			TileColRenderer* firstTile = mColMap->GetTile(first.X, first.Y);
			firstTile->SetActor(this);

			mIsChangingPath = false;
		}
	}
}

void Monster::SetMove()
{
}

void Monster::SetMoveM()
{
}

void Monster::NormalDrop()
{
	int ran = RandomNumber::GetRandomNumber(1, 100);

	if (ran <= 60)
	{
		return;
	}
	else if (ran <= 90)
	{
		DropMoney();
	}
	else
	{
		DropItem();
	}
}

void Monster::DropMoney()
{
	TileIndex myIndex = GetTileIndex();
	TileColRenderer* myTile = mColMap->FindTile(myIndex.X, myIndex.Y);

	for (int i = 0; i < 8; ++i)
	{
		myTile = mColMap->FindTile(myIndex + mColMap->mDirArr[i]);

		if (nullptr == myTile->GetItem() && true == myTile->IsMovable())
		{
			if (1 == mData.dlevel)
			{
				// MyGameSound::SoundPlay(L"gold.wav");

			
				int ran = RandomNumber::GetRandomNumber(1, 21);
				ItemManager::DropMoney(myIndex + mColMap->mDirArr[i], ran, mLevel);

			}
			else
			{
				// MyGameSound::SoundPlay(L"gold.wav");

				int ran = RandomNumber::GetRandomNumber(12, 41);
				ItemManager::DropMoney(myIndex + mColMap->mDirArr[i], ran, mLevel);
			}

			return;
		}
	}
}

void Monster::DropItem()
{
	TileIndex myIndex = GetTileIndex();
	TileColRenderer* myTile = mColMap->FindTile(myIndex.X, myIndex.Y);

	for (int i = 0; i < 8; ++i)
	{
		myTile = mColMap->FindTile(myIndex + mColMap->mDirArr[i]);

		if (nullptr == myTile->GetItem())
		{
			if (1 == mData.dlevel)
			{
				int ran = RandomNumber::GetRandomNumber(1, 14);

				switch (ran)
				{
				case 1:
				// 	MyGameSound::SoundPlay(L"flipswor.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Short Sword", 0, mLevel);

					break;
				case 2:
				// 	MyGameSound::SoundPlay(L"flipswor.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Sabre", 1, mLevel);
					break;
				case 3:			
				// 	MyGameSound::SoundPlay(L"fliplarm.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Rags", 6, mLevel);
					break;
				case 4:
				// 	MyGameSound::SoundPlay(L"fliplarm.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Robe", 7, mLevel);
					break;
				case 5:
				// 	MyGameSound::SoundPlay(L"flipcap.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Cap", 18, mLevel);
					break;
				case 6:
				// 	MyGameSound::SoundPlay(L"flipcap.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Skull Cap", 19, mLevel);
					break;
				case 7:
				// 	MyGameSound::SoundPlay(L"flipshld.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Buckler", 12, mLevel);
					break;
				case 8:
				// 	MyGameSound::SoundPlay(L"flipshld.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Small Shield", 19, mLevel);
					break;
				case 9:
				// 	MyGameSound::SoundPlay(L"flipring.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Ring", 21, mLevel);
					break;
				case 10:
				// 	MyGameSound::SoundPlay(L"flippot.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Potion of Healing", 24, mLevel);
					break;
				case 11:
				// 	MyGameSound::SoundPlay(L"flippot.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Potion of Mana", 25, mLevel);
					break;
				case 12:
					// MyGameSound::SoundPlay(L"flippot.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Potion of Rejuvenation", 26, mLevel);
					break;
				case 13:
					// MyGameSound::SoundPlay(L"flipscrl.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Scroll of Identify", 27, mLevel);
					break;
				case 14:
				// 	MyGameSound::SoundPlay(L"flipscrl.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Scroll of Town Portal", 28, mLevel);
					break;
				default:
					BOOM;
					break;
				}			
			}
			else
			{
				int ran = RandomNumber::GetRandomNumber(1, 23);

				switch (ran)
				{
				case 1:
					// MyGameSound::SoundPlay(L"flipswor.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Short Sword", 0, mLevel);
					break;
				case 2:
					// MyGameSound::SoundPlay(L"flipswor.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Sabre", 1, mLevel);
					break;
				case 3:
					// MyGameSound::SoundPlay(L"fliplarm.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Rags", 6, mLevel);
					break;
				case 4:
					// MyGameSound::SoundPlay(L"fliplarm.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Robe", 7, mLevel);
					break;
				case 5:
					// MyGameSound::SoundPlay(L"flipcap.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Cap", 18, mLevel);
					break;
				case 6:
					// MyGameSound::SoundPlay(L"flipcap.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Skull Cap", 19, mLevel);
					break;
				case 7:
					// MyGameSound::SoundPlay(L"flipshld.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Buckler", 12, mLevel);
					break;
				case 8:
					// MyGameSound::SoundPlay(L"flipshld.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Small Shield", 19, mLevel);
					break;
				case 9:
					// MyGameSound::SoundPlay(L"flipring.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Ring", 21, mLevel);
					break;
				case 10:
					
					// MyGameSound::SoundPlay(L"flippot.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Potion of Healing", 24, mLevel);
					break;
				case 11:
					// MyGameSound::SoundPlay(L"flippot.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Potion of Mana", 25, mLevel);
					break;
				case 12:
				// 	MyGameSound::SoundPlay(L"flippot.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Potion of Rejuvenation", 26, mLevel);
					break;
				case 13:
					// MyGameSound::SoundPlay(L"flipscrl.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Scroll of Identify", 27, mLevel);
					break;
				case 14:
					// MyGameSound::SoundPlay(L"flipscrl.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Scroll of Town Portal", 28, mLevel);
					break;
				case 15:
					// MyGameSound::SoundPlay(L"flipswor.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Scimitar", 2, mLevel);
					break;
				case 16:
					// MyGameSound::SoundPlay(L"flipswor.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Long Sword", 3, mLevel);
					break;
				case 17:
					// MyGameSound::SoundPlay(L"flipswor.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Bastard Sword", 4, mLevel);
					break;
				case 18:
					// MyGameSound::SoundPlay(L"fliplarm.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Leather Armor", 8, mLevel);
					break;
				case 19:
					// MyGameSound::SoundPlay(L"fliplarm.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Studded Leather Armor", 9, mLevel);
					break;
				case 20:
					// MyGameSound::SoundPlay(L"flipcap.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Helm", 20, mLevel);
					break;
				case 21:
					// MyGameSound::SoundPlay(L"flipshld.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Large Shield", 14, mLevel);
					break;
				case 22:
					// MyGameSound::SoundPlay(L"flipring.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Amulet", 22, mLevel);
					break;
				case 23:
					// MyGameSound::SoundPlay(L"flipharm.wav");
					ItemManager::DropItem(myIndex + mColMap->mDirArr[i], L"Chain Mail", 10, mLevel);
					break;
				default:
					BOOM;
					break;
				}
			}	

			return;
		}

		return;
	}
}
