#include <MyMath.h>
#include <MyWindowClass.h>
#include <MyScene.h>
#include <MyInput.h>
#include <MyTime.h>
#include <MyRenderer.h>
#include <MyAnimationRenderer.h>
#include <MyCollider.h>
#include <MyTextureManager.h>
#include <MyBaseUpdater.h>
#include <MyImage.h>
#include <MyDirectory.h>

#include "Player.h"
#include "Bullet.h"
#include "ColliderMap.h"
#include "TileColRenderer.h"
#include "StatRenderer.h"
#include "FogRenderer.h"
#include "DibRoom.h"
#include "RandomMap.h"
#include "Item.h"
#include "InvenBackRenderer.h"
#include "ItemManager.h"
#include "LevelRenderer.h"
#include "FontRenderer.h"
#include "YFontRenderer.h"

#include <RandomNumber.h>

#include <MyFile.h>

Player::Player(ColliderMap* colmap, RandomMap* randomMap)
	: Monster(colmap, randomMap)
	, mState(PS_MAX)
	, mIsDirKey(false)
	, mNewDestination(MyVector2::ZERO)
	, mIsStandingAttack(false)
	, mIsInvenOpen(false)
	, mIsStatOpen(false)
	, mButtonOffset(0)
	, mIsPoint(false)
	, mGoingToPick(false)
	, mNextInvenIndex(0)
{
	mTC = L"total kills : ";
	mImune = L"no magic resistance";

	mLevel = 1;

	mIsItemOn = false;

	mExpVec.reserve(50);

	mInventory = new Item*[40];

	/*mInventory1.reserve(10);
	mInventory2.reserve(10);
	mInventory3.reserve(10);
	mInventory4.reserve(10);*/

	/*for (size_t i = 0; i < 10; ++i)
	{
		mInventory1[i] = nullptr;
		mInventory2[i] = nullptr;
		mInventory3[i] = nullptr;
		mInventory4[i] = nullptr;
	}*/

	for (size_t i = 0; i < 40; ++i)
	{
		mInventory[i] = nullptr;
	}

	mAttackSpeed = 0.04f;

	mData.name = L"Warrior";
	mData.dlevel = 1;
	mData.mLevel = 1;

	mData.mMaxHP = 70;
	mData.mMaxMP = 20;
	mData.armor = 0;
	mData.toHit = 100;
	mData.attackMin = 1;
	mData.attackMax = 4;
	mData.exp = 64;
	mData.gold = 0;

	mCurHP = mData.mMaxHP;
	mCurMP = mData.mMaxMP;

	mRandomMap->SetPlayer(this);

	mDir = RB;
	mPrevDir = RB;





	mAnimationRenderer = CreateAniRenderer(LO_PLAYER);

	mAnimationRenderer->SubSize({ 256, 256 });
	mAnimationRenderer->TransColor(255, 255, 255);
	mAnimationRenderer->SubPos({ 0.f, -20.f });

	mAnimationRenderer->SetRenderStyle(RS_TRANS);

	SetAnimation();

	SetCurrentState(PS_IDLE);

	/////////////////////////////////////////////////// FOG

	mFog = CreateRenderer<FogRenderer>(LO_SMALLFOG);
	mFog->Init(this);

	mInvenBack = CreateRenderer<InvenBackRenderer>(LO_UI35);
	mInvenBack->SetPlayer(this);

	/////////////////////////////////////////////////// UI
	mUI = CreateRenderer<MyRenderer>(LO_UI1);
	mUI->SubPos({ 320, 409 });
	mUI->SubSize({ 640, 143 });
	mUI->CamOnOff();

	int y = (int)(337.f - 143 / 2 - 33); // 233
	int x = 8;

	mUI->TransColor(255, 255, 255);

	mHP = CreateRenderer<MyRenderer>(LO_UI2);
	mHP->SubSize({ 640, 85 });
	mHP->SubPos({ 331.f , 391.f });
	mHP->TransColor(255, 255, 255);
	mHP->CamOnOff();

	mMP = CreateRenderer<MyRenderer>(LO_UI2);
	mMP->SubSize({ 640, 85 });
	mMP->SubPos({ 441.f , 391.f });
	mMP->TransColor(255, 255, 255);
	mMP->CamOnOff();

	mUI->Image(L"DibUI.bmp");
	mHP->Image(L"HP.bmp");
	mMP->Image(L"MP.bmp");

	mUI->SetRenderStyle(RS_TRANS);
	mHP->SetRenderStyle(RS_HP);
	mMP->SetRenderStyle(RS_MP);

	/////////////////////////////////////////////////////////////////////////////

	mInven = CreateRenderer<MyRenderer>(LO_UI0);
	mInven->SubSize({ 320, 352 });
	mInven->SubPos({ 480 , 176 });
	mInven->CamOnOff();
	// mInven->TransColor(255, 255, 255);

	mStats = CreateRenderer<MyRenderer>(LO_UI0);
	mStats->SubSize({ 320, 352 });
	mStats->SubPos({ 160 , 176 });
	mStats->CamOnOff();
	// mStats->TransColor(255, 255, 255);

	mInven->Image(L"DibInven.bmp");
	mStats->Image(L"DibStat.bmp");

	mInven->SetRenderStyle(RS_BIT);
	mStats->SetRenderStyle(RS_BIT);

	mInven->Off();
	mStats->Off();

	///////////////////////////////////////////////////// BUTTON
	mButton = CreateRenderer<MyRenderer>(LO_UI3);
	mButton->Image(L"DibButton.bmp");
	mButton->SetRenderStyle(RS_ALPHA);
	mButton->SubPos({ 0, 0 });
	mButton->SubSize({ 73, 21 });
	mButton->SetBF(100);
	mButton->Off();

	mPointButton = CreateRenderer<MyRenderer>(LO_UI3);
	mPointButton->Image(L"LevelUpButton.bmp");
	mPointButton->ImageIndex(0);
	mPointButton->SetRenderStyle(RS_BIT);
	mPointButton->SubPos({ -260, 160 });
	mPointButton->SubSize({ 41, 22 });
	mPointButton->Off();

	mStrButton = CreateRenderer<MyRenderer>(LO_DEBUG);
	mStrButton->Image(L"LevelUpButton.bmp");
	mStrButton->ImageIndex(0);
	mStrButton->SetRenderStyle(RS_BIT);
	mStrButton->SubPos({ -322, -30 + 4 });
	mStrButton->SubSize({ 41, 22 });
	mStrButton->Off();


	mMagButton = CreateRenderer<MyRenderer>(LO_DEBUG);
	mMagButton->Image(L"LevelUpButton.bmp");
	mMagButton->ImageIndex(0);
	mMagButton->SetRenderStyle(RS_BIT);
	mMagButton->SubPos({ -322, -2 + 4 });
	mMagButton->SubSize({ 41, 22 });
	mMagButton->Off();

	mDexButton = CreateRenderer<MyRenderer>(LO_DEBUG);
	mDexButton->Image(L"LevelUpButton.bmp");
	mDexButton->ImageIndex(0);
	mDexButton->SetRenderStyle(RS_BIT);
	mDexButton->SubPos({ -322, 26 + 4 });
	mDexButton->SubSize({ 41, 22 });
	mDexButton->Off();

	mVitButton = CreateRenderer<MyRenderer>(LO_DEBUG);
	mVitButton->Image(L"LevelUpButton.bmp");
	mVitButton->ImageIndex(0);
	mVitButton->SetRenderStyle(RS_BIT);
	mVitButton->SubPos({ -322, 54 + 4 });
	mVitButton->SubSize({ 41, 22 });
	mVitButton->Off();

	///////////////////////////////////////////////////// FONT
	mStatRenderer = CreateRenderer<StatRenderer>(LO_UI4);
	mStatRenderer->SubSize({ 14, 14 });
	mStatRenderer->SetPlayer(this);
	// mStatRenderer->ClassRender();

	mFontRenderer = CreateRenderer<FontRenderer>(LO_UI4);
	mFontRenderer->SetPlayer(this);
	mFontRenderer->Off();

	mYFontRenderer = CreateRenderer<YFontRenderer>(LO_UI4);
	mYFontRenderer->SetPlayer(this);
	mYFontRenderer->Off();

	////////////////////////////////////////////////////// LEVEL
	mLevelRender = CreateRenderer<LevelRenderer>(LO_UI0);
	mLevelRender->SubSize({ 14, 14 });
	mLevelRender->SetPlayer(this);


	/////////////////////////////////////////////////////// EXP Load
	MyDirectory newDir;

	newDir.MoveParent();


	std::wstring fName = newDir.GetPath();
	fName.append(L"\\Exp.data");

	mExpFile = new MyFile(fName.c_str(), L"rt");
	LoadEXP();

	int a = 0;

	/////////////////////////////////////////////////////// Effect
	mLevelUpRenderer = CreateAniRenderer(LO_PLAYER);
	mLevelUpRenderer->Image(L"LightSource.bmp");
	mLevelUpRenderer->SubPos({ 0.f, -70.f });
	mLevelUpRenderer->SubSize({ 97 * 1.5f, 162 * 1.4f });
	mLevelUpRenderer->TransColor(255, 255, 255);
	mLevelUpRenderer->SetBF(160);
	mLevelUpRenderer->SetAlpha();
	mLevelUpRenderer->SetRenderStyle(RS_ALPHA);
	mLevelUpRenderer->CreateAnimation(L"LevelUp", L"LightSource.bmp", 0, 15, false);

	mLevelUpRenderer->SetCurrentAnimation(L"LevelUp");
	mLevelUpRenderer->Off();

	ItemManager::Init(mColMap);

	SetCurPos(mRandomMap->mStartPos1.X, mRandomMap->mStartPos1.Y + 2);
	SetDir(TileIndex({ mRandomMap->mStartPos1.X, mRandomMap->mStartPos1.Y + 3 }));
}


Player::~Player()
{
	delete mExpFile;

	delete mInventory;
}

void Player::CopyPlayer(Player* player)
{
	// mInventory = new Item*[40];

	std::list<Item*>::iterator iter = mItemList.begin();
	std::list<Item*>::iterator iterEnd = mItemList.end();

	for (; iter != iterEnd; )
	{
		if (nullptr != *iter)
		{
			(*iter)->KillThis();
			iter = mItemList.erase(iter);
			continue;
		}

		++iter;
	}

	mItemList.clear();

	if(nullptr != mWeapon)
	{
		// mWeapon->Unarm();
		mWeapon->KillThis();
		mWeapon = nullptr;
		mMouseItem = nullptr;
		mIsItemOn = false;
	}
	if (nullptr != mWeapon)
	{
		// mArmor->Unarm();
		mArmor->KillThis();
		mArmor = nullptr;
		mMouseItem = nullptr;
		mIsItemOn = false;
	}
	if (nullptr != mWeapon)
	{
		// mShield->Unarm();
		mShield->KillThis();
		mShield = nullptr;
		mMouseItem = nullptr;
		mIsItemOn = false;
	}
	if (nullptr != mWeapon)
	{
		// mHelm->Unarm();
		mHelm->KillThis();
		mHelm = nullptr;
		mMouseItem = nullptr;
		mIsItemOn = false;
	}
	if (nullptr != mWeapon)
	{
		// mLRing->Unarm();
		mLRing->KillThis();
		mLRing = nullptr;
		mMouseItem = nullptr;
		mIsItemOn = false;
	}
	if (nullptr != mWeapon)
	{
		// mRRing->Unarm();
		mRRing->KillThis();
		mRRing = nullptr;
		mMouseItem = nullptr;
		mIsItemOn = false;
	}
	if (nullptr != mWeapon)
	{
		// mNeck->Unarm();
		mNeck->KillThis();
		mNeck = nullptr;
		mMouseItem = nullptr;
		mIsItemOn = false;
	}

	mMouseItem = nullptr;
	mIsItemOn = false;


	for (size_t i = 0; i < 40; i++)
	{
		mInventory[i] = nullptr;
	}

	mData.mLevel = player->mData.mLevel;

	mData.exp = player->mData.exp;
	mData.gold = player->mData.gold;

	mCurHP = player->mCurHP;
	mCurMP = player->mCurMP;

	mItemAttack =0;
	mItemArmor = 0;
	mItemToHit = 0;
	mItemHP = 0;
	mItemMP = 0;
	mItemStr = 0;
	mItemDex = 0;
	mItemVit = 0;
	mItemInt = 0;
	mItemMag = 0;
	mStr = player->mStr					 ;
	mDex = player->mDex					 ;
	mVit = player->mVit					 ;
	mMag = player->mMag					 ;
	mPoint = player->mPoint				 ;
	mIsPoint = player->mIsPoint			 ;
	mHasWeapon = player->mHasWeapon		 ;
	mHasHelm =  player->mHasHelm	 ;
	mHasArmor = player->mHasArmor		 ;
	mHasShield = player->mHasShield		 ;
	mHasLeftRing = player->mHasLeftRing	 ;
	mHasRightRing = player->mHasRightRing;
	mHasNeck = player->mHasNeck			 ;	

	mWeapon = nullptr;
	mArmor = nullptr;
	mShield = nullptr;
	mHelm = nullptr;
	mLRing = nullptr;
	mRRing = nullptr;
	mNeck = nullptr;
}


// 인덱스 -> 월드 포지션
void Player::SetCurPos(int indexX, int indexY)
{
	mTileIndex.X = indexX;
	mTileIndex.Y = indexY;
	// mPrevTileIndex = mTileIndex;

	float x = (indexX * QUARTER_TILE_WIDTH) + (indexY * -QUARTER_TILE_WIDTH);
	float y = (indexX * QUARTER_TILE_HEIGHT) + (indexY * QUARTER_TILE_HEIGHT) - TILE_SUBOFFSETY / 2;


	Pos({ x, y });
	Scene()->CamMove({ x, y });

	Monster::SetUnit();

	mLastRoom = mDibTile->mRoom;

	mStart = Pos();

	SetCurrentState(PS_IDLE);
}

void Player::SetCurrentState(PLAYER_STATE state, bool noneDir /*= false*/)
{
	mState = state;

	std::wstring newState = mAllDirName[mDir];

	if (true == noneDir)
	{
		newState = L"";
	}

	newState.append(L"_");
	newState.append(mAllStateName[state]);

	mAnimationRenderer->SetCurrentAnimation(newState.c_str());
}

float atime = 1.0f;
float fps;

void Player::Update()
{
	// 폰트
	SetMouseMonster();

	if (nullptr != mMouseMonster)
	{
		wchar_t amount[5];
		int kill = mMouseMonster->MonsterCount();
		_itow_s(kill, amount, 5, 10);

		std::wstring temp = mTC;
		temp.append(amount);

		ShowDescription(mMouseMonster->GetUnitData().name, temp, mImune);
	}
	else if (true == mIsShowing1)
	{
		ShowDescription(mString1);
	}
	else if (true == mIsYShowing1)
	{
		ShowYDescription(mString1);
	}
	else if (true == mIsShowing2)
	{
		ShowDescription(mString1, mString2);
	}
	else if (true == mIsYShowing2)
	{
		ShowYDescription(mString1, mString2);
	}
	else
	{
		UnshowDescription();
	}


	// 마우스 기본 동작
	if (true == MyInput::Down(L"LMOUSEBTN"))
	{
		mIsClicked = true;
	}
	/*if (true == MyInput::Press(L"LMOUSEBTN"))
	{

	}*/
	if (true == MyInput::Up(L"LMOUSEBTN") || true == MyInput::Release(L"LMOUSEBTN"))
	{
		mIsClicked = false;
	}


	// 마우스와 바닥에 떨어진 아이템 상호작용
	CheckMouse();

	// 돈 떨어짐
	if (true == MyInput::Down(L"ITEM"))
	{
		/*TileIndex temp0 = GetTileIndex();
		TileIndex temp = GetTileIndex() - 1;
		ItemManager::DropMoney(GetTileIndex() - 1);*/

		ItemManager::DropItem(GetTileIndex() - 1, L"cleaver", 11, mLevel);
		ItemManager::DropItem(GetTileIndex() + 1, L"long sword", 3, mLevel);
	}

	if (true == MyInput::Down(L"MONEY"))
	{
		TileIndex temp0 = GetTileIndex();
		TileIndex temp = GetTileIndex() - 1;
		ItemManager::DropMoney(GetTileIndex() - 1, 101, mLevel);
	}

	// 레벨업 애니메이션
	if (true == mLevelUpRenderer->IsCurrentAnimationEnd())
	{
		mLevelUpRenderer->Off();
	}

	if (0 < mPoint)
	{
		mIsPoint = true;
	}
	else
	{
		mIsPoint = false;
	}

	// 경험치 업 단축키
	if (true == MyInput::Down(L"EXPUP"))
	{
		mData.exp += 2000;
	}

	UpdateStat();

	// true면 UI를 클릭함
	CheckButton();

	if (true == mIsPoint && false == mIsStatOpen)
	{
		mPointButton->On();
	}
	else if (true == mIsStatOpen)
	{
		mPointButton->Off();
	}

	if (true == mIsPoint && true == mIsStatOpen)
	{
		mStrButton->On();
		mDexButton->On();
		mMagButton->On();
		mVitButton->On();
	}
	else if (false == mIsPoint || false == mIsStatOpen)
	{
		mStrButton->Off();
		mDexButton->Off();
		mMagButton->Off();
		mVitButton->Off();
	}

	OpenCloseInven();
	OpenCloseStat();

	if (true == mIsInvenOpen && false == mIsStatOpen)
	{
		Scene()->CamPos(Pos() + MyVector2(-320 + 160.f, -240.f + HALF_TILE_HEIGHT * 2));
		mButtonOffset = 160;
	}
	else if (false == mIsInvenOpen && true == mIsStatOpen)
	{
		Scene()->CamPos(Pos() + MyVector2(-320 - 160.f, -240.f + HALF_TILE_HEIGHT * 2));
		mButtonOffset = -160;
	}
	else
	{
		Scene()->CamPos(Pos() + MyVector2(-320.f, -240.f + HALF_TILE_HEIGHT * 2));
		mButtonOffset = 0;
	}

	// 프레임 출력

	atime -= MyTime::DeltaTime();

	if (atime <= 0)
	{
		fps = 1.f / MyTime::DeltaTime();
		atime = 1.f;

		wchar_t Arr[200] = { 0, };
		swprintf_s(Arr, L"FPS = %f\n", fps);
		OutputDebugStringW(Arr);
	}

	//////////////////////////////////////////////////////////////// Transparent

	TileIndex tileIndex = { mX, mY };

	TileIndex playerTileIndex = DibTileIndex();


	TileIndex tileDis = tileIndex - playerTileIndex;
	TileIndex lastDis;

	lastDis.X = abs(tileDis.X);
	lastDis.Y = abs(tileDis.Y);

	int gap = lastDis.X + lastDis.Y;

	gap = abs(gap);

	if (mLastRoom != mDibTile->mRoom)
	{
		CheckInRoom();
	}

	//////////////////////////////////////////////////////////////// Move

	CheckDir();
	CheckDirChange();
	SetUnit();

	switch (mState)
	{
	case PS_IDLE:
		Idle();
		break;
	case PS_WALK:
		Walk();
		break;
	case PS_ATTACK:
		Attack();
		break;
	case PS_BLOCK:
		Block();
		break;
	case PS_DIE:
		Die();
		break;
	case PS_BLUESKILL:
		BlueSkill();
		break;
	case PS_REDSKILL:
		RedSkill();
		break;
	case PS_GETHIT:
		GetHit();
		break;
	case PS_MAX:
		BOOM;
		break;
	default:
		break;
	}
}

bool Player::CheckDir()
{
	mIsDirKey = true;

	if (mCurDestination == MyVector2::ZERO)
	{
		return false;
	}

	MyVector2 test = Pos();

	float x = Pos().X - mCurDestination.X;
	float y = Pos().Y - mCurDestination.Y;

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

	return true;
}

void Player::CheckDirChange()
{
	if (mDir != mPrevDir && false == mIsAttacking)
	{
		SetCurrentState(mState);

		mPrevDir = mDir;
	}
}

void Player::Idle()
{
	if (true == MyInput::Press(L"SHIFT"))
	{
		if (true == MyInput::Down(L"LMOUSEBTN"))
		{
			// 클릭하면 마우스 위치 저장
			MyVector2 mousePos = MyWindowClass::MainWindow()->MousePos();

			StandingAttack(mousePos);

			return;
		}
	}

	if (true == MyInput::Down(L"LMOUSEBTN"))
	{
		// 경우 1 : 찍은 곳이 아무도 없는 타일
		// 경우 2 : 찍은 곳이 누군가 있는 타일 => 가서 공격해라
		// 경우 3 : 찍은 곳이 아이템이 있는 타일 => 가서 무라

		// 클릭하면 마우스 위치 저장
		MyVector2 mousePos = MyWindowClass::MainWindow()->MousePos();

		// 마우스 움직임 예외 처리
		if (false == CheckMouse(mousePos))
		{
			return;
		}

		// 마우스 위치의 타일 인덱스
		TileIndex destIndex = mColMap->PosToIndex(mousePos);
		// 그 타일 인덱스의 타일
		TileColRenderer* destTile = mColMap->GetTile(destIndex.X, destIndex.Y);


		if (nullptr != destTile)
		{
			// 경우 1
			if (nullptr == destTile->GetActor())
			{
				if (nullptr != destTile->GetItem())
				{
					GoingToPick(mousePos);

					return;
				}

				GoingToMove(mousePos);
			}
			// 경우 2
			else
			{
				// 찍은 곳이 나 자신
				if (this == destTile->GetActor())
				{
					return;
				}				
				// 찍은 곳이 몬스터
				else
				{
					GoingToAttack(mousePos);
				}

			}
		}
	}
}

float wtime = 0.2f;

void Player::Walk()
{
	// SetDir(mCurDestination);
	// PathFind
	MyVector2 temp = MyMath::Lerp(mStart, mCurDestination, MyTime::DeltaTime());

	

	wtime -= MyTime::DeltaTime();

	if (wtime <= 0)
	{	
		// fps = 1.f / MyTime::DeltaTime();
		switch (++mStepCount)
		{
		case 1:
			MyGameSound::SoundPlay(L"walk1.wav");
			break;

		case 2:
			MyGameSound::SoundPlay(L"walk2.wav");
			break;

		case 3:
			MyGameSound::SoundPlay(L"walk3.wav");
			break;

		case 4:
			MyGameSound::SoundPlay(L"walk4.wav");
			mStepCount = 0;
			break;
		default:
			break;
		}

		wtime = 0.2f;
	}




	mIsMoving = true;

	Move(temp * mMoveSpeed);
	Scene()->CamMove(temp * mMoveSpeed);

	if (L == mDir ||
		LT == mDir ||
		LB == mDir)
	{
		if (Pos() <= mCurDestination)
		{
			WalkTask();
		}
	}
	else if (T == mDir)
	{
		if (Pos().X >= mCurDestination.X &&
			Pos().Y <= mCurDestination.Y)
		{
			WalkTask();
		}
	}
	else if (B == mDir)
	{
		if (Pos().X <= mCurDestination.X &&
			Pos().Y >= mCurDestination.Y)
		{
			WalkTask();
		}
	}
	else
	{
		if (Pos() >= mCurDestination)
		{
			WalkTask();
		}
	}

	if (true == MyInput::Press(L"SHIFT"))
	{
		if (true == MyInput::Down(L"LMOUSEBTN"))
		{

			mLastMouse = MyWindowClass::MainWindow()->MousePos();


			mIsStandingAttack = true;

			mPathList.clear();

			return;
		}
	}

	if (true == MyInput::Down(L"LMOUSEBTN"))
	{
		// 클릭하면 마우스 위치 저장
		MyVector2 mousePos = MyWindowClass::MainWindow()->MousePos();

		if (false == CheckMouse(mousePos))
		{
			return;
		}
		else
		{
			// 마우스 위치의 타일 인덱스
			TileIndex destIndex = mColMap->PosToIndex(mousePos);
			// 그 타일 인덱스의 타일
			TileColRenderer* destTile = mColMap->GetTile(destIndex.X, destIndex.Y);

			mPathList.clear();
			mPathList = mColMap->PathFind(mCurDestination - Scene()->CamPos(), mousePos);

			mIsChangingPath = true;
		}
	}
}

// 한칸마다 할 일들
void Player::WalkTask()
{
	// 타일에 도착하면 먼저 deltaTime때문에 오차가 생겼는데 이걸을 없애준다.
	Pos(mCurDestination);
	// 카메라 위치도 오차 없애기
	Scene()->CamPos(Pos() + MyVector2(-320.f + mButtonOffset, -240.f + HALF_TILE_HEIGHT * 2));
	mStart = Pos();

	// 방 체크체크쳌츠케ㅡ
	CheckInRoom();

	// 서서 공격
	if (true == mIsStandingAttack)
	{
		mIsStandingAttack = false;

		StandingAttack(mLastMouse);

		return;
	}


	// 마우스를 계속 누르고 있는 경우에도 적용 가능
	if (true == MyInput::Press(L"LMOUSEBTN"))
	{
		// 클릭하면 마우스 위치 저장
		MyVector2 mousePos = MyWindowClass::MainWindow()->MousePos();

		if (false == CheckMouse(mousePos))
		{
			return;
		}

		// 마우스 위치의 타일 인덱스
		TileIndex destIndex = mColMap->PosToIndex(mousePos);
		// 그 타일 인덱스의 타일
		TileColRenderer* destTile = mColMap->GetTile(destIndex.X, destIndex.Y);

		GoingToMove(mousePos);



		return;
	}

	// 경우 1
	// 이제 도착! => Idle
	if (0 == mPathList.size())
	{
		// 경우 1-2 도착했는데 때리고 싶은 경우!!!!
		if (true == mGoingToAttack)
		{
			// 경우 1-2-1 때리자...
			if (nullptr != mDestDibTile->GetActor() || this != mDestDibTile->GetActor())
			{
				SetDir(mDestDibTile);

				mIsAttacking = true;
				SetCurrentState(PS_ATTACK);

				mGoingToAttack = false;

				return;
			}
			// 경우 1-2-2 없으면 PS_IDLE		
		}

		if (true == mGoingToPick)
		{
			if (nullptr != mDestDibTile->GetItem())
			{
				SetDir(mDestDibTile);

				PickItem(mDestDibTile);

				mGoingToPick = false;

				return;
			}
		}

		SetCurrentState(PS_IDLE);
		mIsMoving = false;
	}
	// 경우 2
	// 아직 갈길이 있다!
	else
	{
		// 찾아놓은 길에서 다음 타일을 빼서 준다.
		// mCurDestination을 업데이트!
		mCurDestination = (*mPathList.begin());
		mPathList.pop_front();

		// -------------------------------------------------------------------------------------------
		// 그 길에 누가 있는지 다시 확인!!!
		// 누가 있으면 멈춘다.
		TileColRenderer* nextTile = mColMap->GetTile(mCurDestination - CamPos());
		TileIndex index = nextTile->GetTileIndex();

		if (nullptr != nextTile->GetActor() && this != nextTile->GetActor())
		{
			SetCurrentState(PS_IDLE);
			mIsMoving = false;

			return;
		}
		// -------------------------------------------------------------------------------------------
		SetDir(mCurDestination);
		// 미리 한칸을 움직여서 겹치는 것을 방지!
		PreMove();
	}
}

void Player::Attack()
{


	if (true == mAnimationRenderer->IsCurrentAnimationEnd())
	{
		mIsAttacking = false;
		mGoingToAttack = false;
		mAttacked = false;
		mMonster = nullptr;
		SetCurrentState(PS_IDLE);
		mPathList.clear();
	}
	else
	{
		// 방향 구하기...
		SetDir(mDestDibTile);

		// 방향 바뀐 것 적용
		CheckDirChange();

		mIsAttacking = true;

		if (0 != mAnimationRenderer->CurFrame() - mDir * 16 &&
			9 == (mAnimationRenderer->CurFrame() - mDir * 16) &&
			false == mAttacked)
		{
			int rand = RandomNumber::GetRandomNumber(1, 2);

			if (1 == rand)
			{
				MyGameSound::SoundPlay(L"swing.wav");
			}
			else
			{
				MyGameSound::SoundPlay(L"swing2.wav");
			}

			int a = mAnimationRenderer->CurFrame();

			switch (mDir)
			{
			case B:
				mMonster = mColMap->GetTile(mTileIndex.X, mTileIndex.Y + 1)->GetActor();
				break;
			case LB:
				mMonster = mColMap->GetTile(mTileIndex.X - 1, mTileIndex.Y + 1)->GetActor();
				break;
			case L:
				mMonster = mColMap->GetTile(mTileIndex.X - 1, mTileIndex.Y)->GetActor();
				break;
			case LT:
				mMonster = mColMap->GetTile(mTileIndex.X - 1, mTileIndex.Y - 1)->GetActor();
				break;
			case T:
				mMonster = mColMap->GetTile(mTileIndex.X, mTileIndex.Y - 1)->GetActor();
				break;
			case RT:
				mMonster = mColMap->GetTile(mTileIndex.X + 1, mTileIndex.Y - 1)->GetActor();
				break;
			case R:
				mMonster = mColMap->GetTile(mTileIndex.X + 1, mTileIndex.Y)->GetActor();
				break;
			case RB:
				mMonster = mColMap->GetTile(mTileIndex.X + 1, mTileIndex.Y + 1)->GetActor();
				break;
			case MD_MAX:
				BOOM;
				break;
			default:
				break;
			}

			mAttacked = true;

			if (nullptr != mMonster)
			{
				int ran = RandomNumber::GetRandomNumber(1, 100);

				if (ran <= mData.toHit)
				{
					mMonster->GetDamage(mData.attackMin);
				}	
			}
		}

		// 여기에 문제가 있는것이로 보인다...?
		// 이제 헛방 친다........ 또잉
		if (0 != mAnimationRenderer->CurFrame() - mDir * 16 &&
			10 <= (mAnimationRenderer->CurFrame() - mDir * 16) &&
			true == mAttacked)
		{
			if (true == MyInput::Press(L"SHIFT"))
			{
				if (true == MyInput::Down(L"LMOUSEBTN"))
				{
					// 클릭하면 마우스 위치 저장

					MyVector2 mousePos = MyWindowClass::MainWindow()->MousePos();

					StandingAttack(mousePos);

					return;
				}
			}
			if (true == MyInput::Down(L"LMOUSEBTN"))
			{
				MyVector2 mousePos = MyWindowClass::MainWindow()->MousePos();
				// 클릭하면 마우스 위치 저장
				if (false == CheckMouse(mousePos))
				{
					return;
				}


				// 마우스 위치의 타일 인덱스
				TileIndex destIndex = mColMap->PosToIndex(mousePos);
				// 그 타일 인덱스의 타일
				TileColRenderer* destTile = mColMap->GetTile(destIndex.X, destIndex.Y);

				mAttacked = false;

				if (nullptr != destTile)
				{
					// 경우 1
					if (nullptr == destTile->GetActor())
					{
						mIsAttacking = false;
						GoingToMove(mousePos);
					}
					// 경우 2
					else
					{
						// 찍은 곳이 나 자신
						if (this == destTile->GetActor())
						{
							mIsAttacking = false;
							return;
						}
						// 찍은 곳이 몬스터
						else
						{
							GoingToAttack(mousePos);
						}

					}
				}
			}
		}


	}
}

void Player::Block()
{
	if (true == mAnimationRenderer->IsCurrentAnimationEnd())
	{
		if (MyInput::Press(L"BLOCK"))
		{
			SetCurrentState(PS_BLOCK);
		}
		else
		{
			SetCurrentState(PS_IDLE);
		}

	}
}

void Player::Die()
{
	if (true == mAnimationRenderer->IsCurrentAnimationEnd())
	{
		if (MyInput::Press(L"DIE"))
		{
			SetCurrentState(PS_DIE);
		}
		else
		{
			SetCurrentState(PS_IDLE);
		}

	}
}

void Player::RedSkill()
{
	if (true == mAnimationRenderer->IsCurrentAnimationEnd())
	{
		if (MyInput::Press(L"REDSKILL"))
		{
			SetCurrentState(PS_REDSKILL);
		}
		else
		{
			SetCurrentState(PS_IDLE);
		}

	}
}

void Player::BlueSkill()
{
	if (true == mAnimationRenderer->IsCurrentAnimationEnd())
	{
		if (MyInput::Press(L"BLUESKILL"))
		{
			SetCurrentState(PS_BLUESKILL);
		}
		else
		{
			SetCurrentState(PS_IDLE);
		}
	}
}

void Player::GetDamage(int damage)
{
	int rand = RandomNumber::GetRandomNumber(1, 2);

	if (1 == rand)
	{
		MyGameSound::SoundPlay(L"lghit.wav");
	}
	else
	{
		MyGameSound::SoundPlay(L"lghit1.wav");

	}

	mCurHP -= damage;

	mIsAttacking = false;

	SetCurrentState(PS_GETHIT);

	ResetPos(this);
}

void Player::GetHit()
{
	if (true == mAnimationRenderer->IsCurrentAnimationEnd())
	{
		SetCurrentState(PS_IDLE);
	}
}

void Player::Fire()
{
	POINT cursor;

	if (GetCursorPos(&cursor))
	{
		// OutputDebugStringW(L"Hello\n");
	}

	ScreenToClient(MyWindowClass::MainHWND(), &cursor);

	// 단위 벡터를 구해서 넘겨주자.
	int x = cursor.x - (int)mRect.Pos.X;
	int y = cursor.y - (int)mRect.Pos.Y;

	float temp = sqrtf((float)x * x + (float)y * y);

	float newX = x / temp;
	float newY = y / temp;

	MyVector2 newDir = MyVector2(newX, newY);

	Bullet* newBullet = new Bullet(newDir);
	newBullet->Pos(Pos());
}

void Player::StandingAttack(MyVector2 pos)
{
	CalDir(pos);

	mAttacked = false;
	mIsAttacking = true;
	SetCurrentState(PS_ATTACK);
}

bool Player::CheckWalkableTile()
{
	switch (mDir)
	{
	case RB:
		if (nullptr == mColMap->CheckTile(mTileIndex.X + 1, mTileIndex.Y + 1))
		{
			return false;
		}
		break;
	case B:
		if (nullptr == mColMap->CheckTile(mTileIndex.X, mTileIndex.Y + 1))
		{
			return false;
		}
		break;
	case LB:
		if (nullptr == mColMap->CheckTile(mTileIndex.X - 1, mTileIndex.Y + 1))
		{
			return false;
		}
		break;
	case L:
		if (nullptr == mColMap->CheckTile(mTileIndex.X - 1, mTileIndex.Y))
		{
			return false;
		}
		break;
	case LT:
		if (nullptr == mColMap->CheckTile(mTileIndex.X - 1, mTileIndex.Y - 1))
		{
			return false;
		}
		break;
	case T:
		if (nullptr == mColMap->CheckTile(mTileIndex.X, mTileIndex.Y - 1))
		{
			return false;
		}
		break;
	case RT:
		if (nullptr == mColMap->CheckTile(mTileIndex.X + 1, mTileIndex.Y - 1))
		{
			return false;
		}
		break;
	case R:
		if (nullptr == mColMap->CheckTile(mTileIndex.X + 1, mTileIndex.Y))
		{
			return false;
		}
		break;
	case MD_MAX:
		break;
	default:
		break;
	}

	return true;

}

void Player::ShowDescription(std::wstring string)
{
	mFontRenderer->On();
	mFontRenderer->SetString(string);

	mIsShowing1 = false;
	mIsShowing2 = false;
	mIsShowing3 = false;
}

void Player::ShowDescription(std::wstring string1, std::wstring string2)
{
	mFontRenderer->On();
	mFontRenderer->SetString(string1, string2);

	mIsShowing1 = false;
	mIsShowing2 = false;
	mIsShowing3 = false;
}

void Player::ShowDescription(std::wstring string1, std::wstring string2, std::wstring string3)
{
	mFontRenderer->On();
	mFontRenderer->SetString(string1, string2, string3);

	mIsShowing1 = false;
	mIsShowing2 = false;
	mIsShowing3 = false;
}

void Player::ShowYDescription(std::wstring string)
{
	mYFontRenderer->On();
	mYFontRenderer->SetString(string);

	mIsYShowing1 = false;
	mIsYShowing2 = false;
	mIsYShowing3 = false;
}

void Player::ShowYDescription(std::wstring string1, std::wstring string2)
{
	mYFontRenderer->On();
	mYFontRenderer->SetString(string1, string2);

	mIsYShowing1 = false;
	mIsYShowing2 = false;
	mIsYShowing3 = false;
}

void Player::ShowYDescription(std::wstring string1, std::wstring string2, std::wstring string3)
{
	mYFontRenderer->On();
	mYFontRenderer->SetString(string1, string2, string3);

	mIsYShowing1 = false;
	mIsYShowing2 = false;
	mIsYShowing3 = false;
}

void Player::UnshowDescription()
{
	mFontRenderer->Off();
	mYFontRenderer->Off();

	mIsYShowing1 = false;
	mIsYShowing2 = false;
	mIsYShowing3 = false;
}

void Player::SetAnimation()
{
	mAllStateName[PS_IDLE] = L"IDLE";
	mAllStateName[PS_WALK] = L"WALK";
	mAllStateName[PS_ATTACK] = L"ATTACK";
	mAllStateName[PS_BLOCK] = L"BLOCK";
	mAllStateName[PS_DIE] = L"DIE";
	mAllStateName[PS_BLUESKILL] = L"BLUESKILL";
	mAllStateName[PS_REDSKILL] = L"REDSKILL";
	mAllStateName[PS_GETHIT] = L"GETHIT";

	mAllDirName[T] = L"T";
	mAllDirName[LT] = L"LT";
	mAllDirName[L] = L"L";
	mAllDirName[LB] = L"LB";
	mAllDirName[B] = L"B";
	mAllDirName[RB] = L"RB";
	mAllDirName[R] = L"R";
	mAllDirName[RT] = L"RT";

	AutoAnimation(L"IDLE", 9, 0.1f);
	AutoAnimation(L"WALK", 7, 0.05f);
	AutoAnimation(L"ATTACK", 15, mAttackSpeed);
	AutoAnimation(L"REDSKILL", 19);
	AutoAnimation(L"BLUESKILL", 19);
	AutoAnimation(L"BLOCK", 1);
	AutoAnimation(L"GETHIT", 5, 0.05f, false);

	mAnimationRenderer->CreateAnimation(L"B_DIE", L"PW&LA&SS&DIE.bmp", 0, 7, false, 0.2f);
	mAnimationRenderer->CreateAnimation(L"LB_DIE", L"PW&LA&SS&DIE.bmp", 8, 15, false, 0.2f);
	mAnimationRenderer->CreateAnimation(L"L_DIE", L"PW&LA&SS&DIE.bmp", 16, 23, false, 0.2f);
	mAnimationRenderer->CreateAnimation(L"LT_DIE", L"PW&LA&SS&DIE.bmp", 24, 31, false, 0.2f);
	mAnimationRenderer->CreateAnimation(L"T_DIE", L"PW&LA&SS&DIE.bmp", 32, 39, false, 0.2f);
	mAnimationRenderer->CreateAnimation(L"RT_DIE", L"PW&LA&SS&DIE.bmp", 40, 47, false, 0.2f);
	mAnimationRenderer->CreateAnimation(L"R_DIE", L"PW&LA&SS&DIE.bmp", 48, 55, false, 0.2f);
	mAnimationRenderer->CreateAnimation(L"RB_DIE", L"PW&LA&SS&DIE.bmp", 56, 63, false, 0.2f);



}

void Player::AutoAnimation(const wchar_t * name, int index, float frameSpeed, bool isLoop)
{
	int start = 0;
	int next = index;

	std::wstring fileName = L"PW&LA&SS&";
	fileName.append(name);
	fileName.append(L".bmp");

	for (int i = 0; i < 8; ++i)
	{
		std::wstring aniName = mAllDirName[i];

		aniName.append(L"_");
		aniName.append(name);



		mAnimationRenderer->CreateAnimation(aniName.c_str(), fileName.c_str(), start, next, true, frameSpeed);

		start = next + 1;
		next = start + index;
	}
}


// 1. mCurDestination => 나의 다음 타일을 설정한다.
// 2. 다음 타일을 미리 점유한다.(다른 캐릭과 겹치기 방지)
// 예상 문제 -> 이동하려고 하면 다음 타일을 미리 점유하지만 맞으면 제잘
// 이를 해결하기 위해서 맞으면 점유한 타일을 모두 Reset한다. => ResetPos();
void Player::GoingToMove(MyVector2 mousePos)
{
	mPathList.clear();
	// 길찾기
	mPathList = mColMap->PathFind(CalPos(), mousePos);

	// 길을 못 찾음
	if (0 == mPathList.size())
	{
		SetCurrentState(PS_IDLE);

		return;
	}

	// 현재 타일은 제거...
	mPathList.pop_front();

	// 나의 다음 타일
	mCurDestination = (*mPathList.begin());
	// 나의 다음 타일 사용했으니 또 제거
	mPathList.pop_front();

	if (mStart == mCurDestination)
	{
		// 나의 다음 타일
		mCurDestination = (*mPathList.begin());
		// 나의 다음 타일 사용했으니 또 제거
		mPathList.pop_front();
	}


	// 걷기
	SetDir(mCurDestination);
	SetCurrentState(PS_WALK);

	// 미리 한칸을 움직여서 겹치는 것을 방지!
	PreMove();
}

// 찍은 곳이 몬스터가 있는곳.
// 마지막 타일을 pop_back한다.
void Player::GoingToAttack(MyVector2 mousePos)
{
	mPathList.clear();
	// 길찾기
	mPathList = mColMap->PathFind(CalPos(), mousePos);

	// 길을 못 찾음
	if (0 == mPathList.size())
	{
		SetCurrentState(PS_IDLE);

		return;
	}

	// 몹이 바로 앞!!!
	// 동시에 몹 쪽으로 방향 전환이 필요하다.
	if (2 == mPathList.size())
	{
		SetDir((*--mPathList.end()));

		mIsAttacking = true;
		SetCurrentState(PS_ATTACK);

		return;
	}

	// 현재 타일은 제거...
	mPathList.pop_front();

	// 나의 다음 타일
	mCurDestination = (*mPathList.begin());
	// 나의 다음 타일 사용했으니 또 제거
	mPathList.pop_front();

	//-----------------------------------------------------------------------------------------

	// GoingToMove와 다른 점!!! 마지막 위치 = 몬스터가 있음
	// 따라서 목적지는 몬스터의 옆 타일이 된다. 
	// 마우스 위치의 타일 인덱스
	TileIndex destIndex = mColMap->PosToIndex(mousePos);
	// 그 타일 인덱스의 타일
	mDestDibTile = mColMap->GetTile(destIndex.X, destIndex.Y);

	// 몬스터 타일 빼기
	mPathList.pop_back();

	// 마지막에 가서 때리겠다는 뜻
	mGoingToAttack = true;

	//-----------------------------------------------------------------------------------------


	// 걷기
	SetDir(mCurDestination);
	SetCurrentState(PS_WALK);

	// 미리 한칸을 움직여서 겹치는 것을 방지!
	PreMove();
}

void Player::GoingToPick(MyVector2 mousePos)
{
	mPathList.clear();
	// 길찾기
	mPathList = mColMap->PathFind(CalPos(), mousePos);

	// 길을 못 찾음
	if (0 == mPathList.size())
	{
		SetCurrentState(PS_IDLE);

		return;
	}

	// 몹이 바로 앞!!!
	// 동시에 몹 쪽으로 방향 전환이 필요하다.
	if (2 == mPathList.size())
	{
		SetDir((*--mPathList.end()));

		PickItem(mousePos);

		return;
	}

	// 현재 타일은 제거...
	mPathList.pop_front();

	// 나의 다음 타일
	mCurDestination = (*mPathList.begin());
	// 나의 다음 타일 사용했으니 또 제거
	mPathList.pop_front();

	//-----------------------------------------------------------------------------------------

	// GoingToMove와 다른 점!!! 마지막 위치 = 몬스터가 있음
	// 따라서 목적지는 몬스터의 옆 타일이 된다. 
	// 마우스 위치의 타일 인덱스
	TileIndex destIndex = mColMap->PosToIndex(mousePos);
	// 그 타일 인덱스의 타일
	mDestDibTile = mColMap->GetTile(destIndex.X, destIndex.Y);

	// 몬스터 타일 빼기
	mPathList.pop_back();

	// 마지막에 가서 때리겠다는 뜻
	mGoingToPick = true;
	//-----------------------------------------------------------------------------------------


	// 걷기
	SetDir(mCurDestination);
	SetCurrentState(PS_WALK);

	// 미리 한칸을 움직여서 겹치는 것을 방지!
	PreMove();
}

void Player::PickItem(MyVector2 mousePos)
{
	mMouseIndex = mColMap->PosToIndex(mousePos);

	TileColRenderer* tempTile = mColMap->FindTile(mMouseIndex.X, mMouseIndex.Y);

	PickItem(tempTile);
}

void Player::PickItem(TileColRenderer* tile)
{
	Item* item = tile->GetItem();



	ITEM_TYPE type = item->mItemData.mType;

	int test = -1;
	test = CheckAcquire(item);

	if (-1 != test)
	{
		Acquire(item, test);
	}
	else
	{
		// 공간이 없다
		int rand = RandomNumber::GetRandomNumber(1, 3);

		if (1 == rand)
		{
			MyGameSound::SoundPlay(L"Wario14b.wav");
		}
		else if (2 == rand)
		{
			MyGameSound::SoundPlay(L"Wario14c.wav");
		}
		else
		{
			MyGameSound::SoundPlay(L"Wario15b.wav");
		}

		

		item->ResetAni();
		return;
	}

	item->AllOff();
	tile->UnsetItem();

	switch (type)
	{
	case IT_GOLD:
		mData.gold += item->mItemData.gold;			
		break;
	case IT_WEAPON_AXE:
		break;
	case IT_WEAPON_CLEAVER:
		break;
	case IT_WEAPON_SWORD:
		break;
	case IT_WEAPON_BOW:
		break;
	case IT_ARMOR_LIGHT:
		break;
	case IT_ARMOR_HEAVY:
		break;
	case IT_HELM:
		break;
	case IT_SHIELD:
		break;
	case IT_RING:
		break;
	case IT_NECK:
		break;
	case IT_BOOK:
		break;
	case IT_HPOTION:
		break;
	case IT_MPOTION:
		break;
	case IT_RPOTION:
		break;
	case IT_ISCROLL:
		break;
	case IT_PSCROLL:
		break;
	case IT_MAX:
		break;
	default:
		break;
	}
}

// 미리 한칸을 움직여서 겹치는 것을 방지!
void Player::PreMove()
{
	MyVector2 first = mCurDestination;
	first -= CamPos();
	TileColRenderer* firstTile = mColMap->GetTile(first.X, first.Y);
	firstTile->SetActor(this);
}

// 방이 바뀌면 이 함수를 다시 실행
void Player::CheckInRoom()
{
	if (nullptr == mLastRoom)
	{
		// return;
	}

	MyVector2 LB = mLastRoom->LeftDown();
	MyVector2 RB = mLastRoom->RightDown();
	MyVector2 RT = mLastRoom->RightTop();

	for (int j = LB.IntX(); j < (RB.X - 1); ++j)
	{
		mRandomMap->mAllCopyTile[RB.IntY() - 1][j]->mWallRenderer->SetBF(255);
	}
	for (int i = RT.IntY(); i < RB.Y; ++i)
	{
		mRandomMap->mAllCopyTile[i][RT.IntX() - 1]->mWallRenderer->SetBF(255);
		mRandomMap->mAllTile[i][RT.IntX() - 1]->mWallRenderer->SetBF(255);
	}

	for (size_t i = 0; i < mRandomMap->GetAllRoom().size(); ++i)
	{
		if (mDibTile->mRoom == (mRandomMap->GetAllRoom()[i]))
		{
			LB = mDibTile->mRoom->LeftDown();
			RB = mDibTile->mRoom->RightDown();
			RT = mDibTile->mRoom->RightTop();

			for (int j = LB.IntX(); j < RB.X - 1; ++j)
			{
				mRandomMap->mAllCopyTile[RB.IntY() - 1][j]->mWallRenderer->SetBF(100);
			}

			for (int k = RT.IntY(); k < RB.Y; ++k)
			{
				mRandomMap->mAllTile[k][RT.IntX() - 1]->mWallRenderer->SetBF(100);
				mRandomMap->mAllCopyTile[k][RT.IntX() - 1]->mWallRenderer->SetBF(100);
			}

			mLastRoom = mDibTile->mRoom;

			return;
		}
	}
}

bool Player::CheckButton()
{
	MyVector2 mousePos = MyWindowClass::MainWindow()->MousePos();

	int x = 10;
	int y = 371;


	if (true == MyInput::Press(L"LMOUSEBTN"))
	{
		// 레벨업 버튼!
		if (true == mIsStatOpen && true == mIsPoint)
		{
			if (mousePos.X >= 138 && mousePos.X <= 138 + 41 && mousePos.Y >= 140 && mousePos.Y <= 140 + 22)
			{
				// MyGameSound::SoundPlay(L"invgrab.wav");
				mStrButton->ImageIndex(1);
				return true;
			}
			else if (mousePos.X >= 138 && mousePos.X <= 138 + 41 && mousePos.Y >= 140 + 22 * 1 + 6 && mousePos.Y <= 140 + 22 * 2 + 6)
			{
				// MyGameSound::SoundPlay(L"invgrab.wav");
				mMagButton->ImageIndex(1);
				return true;
			}
			else if (mousePos.X >= 138 && mousePos.X <= 138 + 41 && mousePos.Y >= 140 + 22 * 2 + 6 * 2 && mousePos.Y <= 140 + 22 * 3 + 6 * 2)
			{
				// MyGameSound::SoundPlay(L"invgrab.wav");
				mDexButton->ImageIndex(1);
				return true;
			}
			else if (mousePos.X >= 138 && mousePos.X <= 138 + 41 && mousePos.Y >= 140 + 22 * 3 + 6 * 3 && mousePos.Y <= 140 + 22 * 4 + 6 * 3)
			{
				// MyGameSound::SoundPlay(L"invgrab.wav");
				mVitButton->ImageIndex(1);
				return true;
			}
		}

		// Stat
		if (mousePos.X >= x && mousePos.X <= x + 73 && mousePos.Y >= y && mousePos.Y <= y + 21)
		{
			// MyGameSound::SoundPlay(L"invgrab.wav");

			mButton->On();

			mButton->SubPos({ -274 + mButtonOffset, 203 });

			return true;
		}
		// Quest
		else if (mousePos.X >= x && mousePos.X <= x + 73 && mousePos.Y >= y + 22 && mousePos.Y <= y + 21 + 21)
		{
			// MyGameSound::SoundPlay(L"invgrab.wav");

			mButton->On();

			mButton->SubPos({ -274 + mButtonOffset, 227 });

			return true;
		}
		// Map
		else if (mousePos.X >= x && mousePos.X <= x + 73 && mousePos.Y >= y + 21 * 3 - 2 && mousePos.Y <= y + 21 * 4 - 2)
		{
			// MyGameSound::SoundPlay(L"invgrab.wav");

			mButton->On();

			mButton->SubPos({ -274 + mButtonOffset, 265 });

			return true;
		}
		// Menu
		else if (mousePos.X >= x && mousePos.X <= x + 73 && mousePos.Y >= y + 21 * 4 + 2 && mousePos.Y <= y + 21 * 5 + 2)
		{
			// MyGameSound::SoundPlay(L"invgrab.wav");

			mButton->On();

			mButton->SubPos({ -274 + mButtonOffset, 288 });

			return true;
		}
		// 반대편
		// 인벤토리
		else if (mousePos.X >= x + 550 && mousePos.X <= x + 550 + 73 && mousePos.Y >= y && mousePos.Y <= y + 21)
		{
			// MyGameSound::SoundPlay(L"invgrab.wav");

			mButton->On();

			mButton->SubPos({ 276 + mButtonOffset, 203 });

			return true;
		}
		// 스펠
		else if (mousePos.X >= x + 550 && mousePos.X <= x + 550 + 73 && mousePos.Y >= y + 22 && mousePos.Y <= y + 21 + 21)
		{
			// MyGameSound::SoundPlay(L"invgrab.wav");

			mButton->On();

			mButton->SubPos({ 276 + mButtonOffset, 227 });

			return true;
		}
		// 스펠 창
		else if (mousePos.X >= x + 550 + 6 && mousePos.X <= x + 550 + 73 - 12 && mousePos.Y >= y + 21 * 2 + 10 && mousePos.Y <= y + 21 * 3 + 34)
		{
			// MyGameSound::SoundPlay(L"invgrab.wav");

			mButton->On();

			mButton->SubPos({ 276 + mButtonOffset, 227 + 31 });

			return true;
		}
		// 물약 부분
		else if (mousePos.X >= x + 195 && mousePos.X <= x + 195 + 26 && mousePos.Y >= y - 3 && mousePos.Y <= y - 3 + 22)
		{
			mButton->On();

			mButton->SubPos({ 0 + mButtonOffset, 0 });

			return true;
		}
		else if (mousePos.X >= x + 195 + 26 + 3 && mousePos.X <= x + 195 + 3 + 26 * 2 && mousePos.Y >= y - 3 && mousePos.Y <= y - 3 + 22)
		{
			mButton->On();

			mButton->SubPos({ 30 + mButtonOffset, 0 });

			return true;
		}
		else if (mousePos.X >= x + 195 + 26 * 2 + 3 * 2 && mousePos.X <= x + 195 + 3 * 2 + 26 * 3 && mousePos.Y >= y - 3 && mousePos.Y <= y - 3 + 22)
		{
			mButton->On();

			mButton->SubPos({ 30 * 2 + mButtonOffset, 0 });

			return true;
		}
		else if (mousePos.X >= x + 195 + 26 * 3 + 3 * 3 && mousePos.X <= x + 195 + 3 * 3 + 26 * 4 && mousePos.Y >= y - 3 && mousePos.Y <= y - 3 + 22)
		{
			mButton->On();

			mButton->SubPos({ 30 * 3 + mButtonOffset, 0 });

			return true;
		}
		else if (mousePos.X >= x + 195 + 26 * 4 + 3 * 4 && mousePos.X <= x + 195 + 4 * 3 + 26 * 5 && mousePos.Y >= y - 3 && mousePos.Y <= y - 3 + 22)
		{
			mButton->On();

			mButton->SubPos({ 30 * 4 + mButtonOffset, 0 });

			return true;

		}
		else if (mousePos.X >= x + 195 + 26 * 5 + 3 * 5 && mousePos.X <= x + 195 + 5 * 3 + 26 * 6 && mousePos.Y >= y - 3 && mousePos.Y <= y - 3 + 22)
		{
			mButton->On();

			mButton->SubPos({ 30 * 5 + mButtonOffset, 0 });

			return true;

		}
		else if (mousePos.X >= x + 195 + 26 * 6 + 3 * 6 && mousePos.X <= x + 195 + 6 * 3 + 26 * 7 && mousePos.Y >= y - 3 && mousePos.Y <= y - 3 + 22)
		{
			mButton->On();

			mButton->SubPos({ 30 * 6 + mButtonOffset, 0 });

			return true;

		}
		else if (mousePos.X >= x + 195 + 26 * 7 + 3 * 7 && mousePos.X <= x + 195 + 7 * 3 + 26 * 8 && mousePos.Y >= y - 3 && mousePos.Y <= y - 3 + 22)
		{
			mButton->On();

			mButton->SubPos({ 30 * 7 + mButtonOffset, 0 });

			return true;
		}
		// mPointButton->SubPos({ -260, 160 });
		// mPointButton->SubSize({ 41, 22 });
		else if (mousePos.X >= 36 && mousePos.X <= 36 + 41 && mousePos.Y >= 165 + 160 && mousePos.Y <= 165 + 160 + 22 && mIsPoint == true)
		{
			mPointButton->ImageIndex(1);

			return true;
		}
	}

	int sizeX = mStatRenderer->SubSize().IntX();
	int sizeY = mStatRenderer->SubSize().IntY();

	if (true == MyInput::Up(L"LMOUSEBTN"))
	{
		mButton->Off();
		mStrButton->ImageIndex(0);
		mMagButton->ImageIndex(0);
		mDexButton->ImageIndex(0);
		mVitButton->ImageIndex(0);
		if (mousePos.X >= x + 550 && mousePos.X <= x + 550 + 73 && mousePos.Y >= y && mousePos.Y <= y + 21)
		{
			MyGameSound::SoundPlay(L"invgrab.wav");

			mIsInvenOpen = !mIsInvenOpen;

			return true;
		}
		else if (mousePos.X >= x && mousePos.X <= x + 73 && mousePos.Y >= y && mousePos.Y <= y + 21)
		{
			MyGameSound::SoundPlay(L"invgrab.wav");

			mIsStatOpen = !mIsStatOpen;

			return true;
		}
		// 레벨 업 버튼
		else if (mousePos.X >= 36 && mousePos.X <= 36 + 41 && mousePos.Y >= 165 + 160 && mousePos.Y <= 165 + 160 + 22 && mIsPoint == true)
		{
			MyGameSound::SoundPlay(L"invgrab.wav");

			if (false == mIsStatOpen)
			{

				mIsStatOpen = !mIsStatOpen;
				mPointButton->Off();
			}

			mPointButton->ImageIndex(0);

			return true;
		}

		// str
		else if (mousePos.X >= 138 && mousePos.X <= 138 + 41 && mousePos.Y >= 140 && mousePos.Y <= 140 + 22 && true == mIsPoint)
		{
			MyGameSound::SoundPlay(L"invgrab.wav");
			++mStr;
			--mPoint;
		}// mag
		else if (mousePos.X >= 138 && mousePos.X <= 138 + 41 && mousePos.Y >= 140 + 22 * 1 + 6 && mousePos.Y <= 140 + 22 * 2 + 6 && true == mIsPoint)
		{
			MyGameSound::SoundPlay(L"invgrab.wav");
			++mMag;
			--mPoint;
		}// dex
		else if (mousePos.X >= 138 && mousePos.X <= 138 + 41 && mousePos.Y >= 140 + 22 * 2 + 6 * 2 && mousePos.Y <= 140 + 22 * 3 + 6 * 2 && true == mIsPoint)
		{
			MyGameSound::SoundPlay(L"invgrab.wav");
			++mDex;
			--mPoint;
		}// vit
		else if (mousePos.X >= 138 && mousePos.X <= 138 + 41 && mousePos.Y >= 140 + 22 * 3 + 6 * 3 && mousePos.Y <= 140 + 22 * 4 + 6 * 3 && true == mIsPoint)
		{
			MyGameSound::SoundPlay(L"invgrab.wav");
			++mVit;
			--mPoint;
		}
	}



	return false;
}

bool Player::CheckMouse()
{
	MyVector2 mousePos = MyWindowClass::MainWindow()->MousePos();

	mMouseIndex = mColMap->PosToIndex(mousePos);

	/*TileColRenderer* tempTile = mColMap->FindTile(mMouseIndex.X, mMouseIndex.Y);

	if (nullptr == tempTile)
	{
		return false;
	}

	Item* item = tempTile->GetItem();

	if (nullptr == item)
	{
		return false;
	}*/

	return true;
}

void Player::OpenCloseInven()
{
	if (true == mIsInvenOpen)
	{
		mInven->On();
	}
	else
	{
		mInven->Off();
	}
}

void Player::OpenCloseStat()
{
	if (true == mIsStatOpen)
	{
		mStats->On();
	}
	else
	{
		mStats->Off();
	}
}

// UI에 걸리면 false
bool Player::CheckMouse(MyVector2 mouse)
{
	bool inven = mIsInvenOpen;
	bool stat = mIsStatOpen;

	// UI에 마우스 위치가 걸린 경우!
	if (mouse.Y + mUI->SubSize().Y / 2 - 20 >= mUI->SubPos().Y)
	{
		return false;
	}
	else if (mouse.X >= mInven->SubPos().X - 160 && true == mIsInvenOpen)
	{
		return false;
	}
	else if (mouse.X >= mStats->SubPos().X - 160 && mouse.X <= mStats->SubPos().X + mStats->SubSize().X - 160 && true == mIsStatOpen)
	{
		return false;
	}
	else if (mouse.X >= 36 && mouse.X <= 36 + 41 && mouse.Y >= 165 + 160 && mouse.Y <= 165 + 160 + 22 && mIsPoint == true)
	{
		return false;
	}

	return true;
}

void Player::UpdateStat()
{
	float rate = (float)(mData.mMaxHP - mCurHP) / (float)mData.mMaxHP;
	mHP->SetHPRate(rate);

	rate = (mData.mMaxMP - mCurMP) / (float)mData.mMaxMP;
	mMP->SetMPRate(rate);

	CheckExp();

	mCurStr = mStr + mItemStr;
	mCurDex = mDex + mItemDex;
	mCurVit = mVit + mItemVit;
	mCurMag = mMag + mItemMag;

	mData.mMaxHP = mBaseHP + 2 * (mVit - 25) + (2 * mItemVit) + mItemHP + (2 * mData.mLevel) + 18;
	mData.mMaxMP = mBaseMP + (mMag - 10) + mItemMag + mData.mLevel + mItemMP - 1;
	mData.armor = (int)(mCurDex / 5.f) + mItemArmor;
	mData.attackMin = (int)(((mCurStr * mData.mLevel) / 100.f) + mItemAttack);
	mData.attackMax = (int)((((mCurStr * mData.mLevel) / 100.f) + mItemAttack) * 1.3);
	mData.toHit = 40 + int(mCurDex / 2.f + mItemToHit + mData.mLevel);

	if (mData.attackMin <= 0)
	{
		mData.attackMin = 1;
	}
	if (mData.attackMax <= mData.attackMin)
	{
		mData.attackMax = mData.attackMin + 1;
	}
}



void Player::CheckExp()
{
	if (mData.exp >= mExpVec[mData.mLevel - 1])
	{
		++mData.mLevel;
		LevelUp();

		mCurHP = mData.mMaxHP;
		mCurMP = mData.mMaxMP;
	}
}

void Player::LevelUp()
{
	MyGameSound::SoundPlay(L"resur.wav");

	mData.mMaxHP += 2;
	mData.mMaxMP += 1;

	mPoint += 5;

	mLevelUpRenderer->On();
	mLevelUpRenderer->SetCurrentAnimation(L"LevelUp");
}

void Player::ResetPos(Monster* player)
{
	Pos(mStart);

	if (true == mIsInvenOpen && false == mIsStatOpen)
	{
		Scene()->CamPos(Pos() + MyVector2(-320 + 160.f, -240.f + HALF_TILE_HEIGHT * 2));
		mButtonOffset = 160;
	}
	else if (false == mIsInvenOpen && true == mIsStatOpen)
	{
		Scene()->CamPos(Pos() + MyVector2(-320 - 160.f, -240.f + HALF_TILE_HEIGHT * 2));
		mButtonOffset = -160;
	}
	else
	{
		Scene()->CamPos(Pos() + MyVector2(-320.f, -240.f + HALF_TILE_HEIGHT * 2));
		mButtonOffset = 0;
	}

	TileIndex cur = GetTileIndex();

	for (size_t i = 0; i < 8; ++i)
	{
		TileIndex temp = cur + ColliderMap::mDirArr[i];

		if (player == mColMap->GetTile(temp.X, temp.Y)->GetActor())
		{
			mColMap->GetTile(temp.X, temp.Y)->UnsetActor();
		}
	}

	mPathList.clear();
}

int Player::CheckAcquire(Item * item)
{
	int count = 0;
	int max = item->mItemData.X * item->mItemData.Y;
	int base = 0;
	bool over = false;

	for (int z = 0; z < 4; z++)
	{
		for (int x = 0; x < 10; ++x)
		{
			for (int y = z; y < z + item->mItemData.Y; ++y)
			{
				if (z + item->mItemData.Y >= 4)
				{
					return -1;
				}

				int a = x + 10 * y;

				if (nullptr == mInventory[x + 10 * y])
				{
					++count;

					if (count == max)
					{
						return x + 10 * y;
					}
				}
				else
				{
					count = 0;
					break;
				}
			}
		}
	}
	
	return -1;
}

// 00 10 01 11 = base
// 0  10  1  11
void Player::Acquire(Item * item, int base)
{
	MyGameSound::SoundPlay(L"invgrab.wav");

	item->mBase = base;

	mItemList.push_back(item);

	int Y = (int)(base / 10);
	int X = base - Y * 10;
	int baseX = X - item->mItemData.X + 1;
	int baseY = Y - item->mItemData.Y + 1;
	ITEM_TYPE type = item->mItemData.mType;
	item->mIsTaken = true;


	if (type == IT_GOLD)
	{
		for (int x = 0; x < 10; ++x)
		{
			for (int y = 0; y < 4; ++y)
			{
				if (nullptr == mInventory[x + 10 * y])
				{					
				}
				else if (mInventory[x + 10 * y]->mItemData.mType == IT_GOLD && mInventory[x + 10 * y]->mItemData.gold < 5000)
				{				
					mInventory[x + 10 * y]->mItemData.gold += item->mItemData.gold;

					if (mInventory[x + 10 * y]->mItemData.gold > 5000)
					{
						item->mItemData.gold = mInventory[x + 10 * y + 1]->mItemData.gold - 5000;
						mInventory[x + 10 * y]->mItemData.gold = 5000;		

						Acquire(item, base);

						item->SetPosition(baseX, baseY);

						return;
					}
					else
					{						
						item->KillThis();

						return;
					}
				}
			}
		}

		mInventory[base] = item;
	}	

	item->SetPosition(baseX, baseY);
}

void Player::SuperAcquire(Item * item, int base)
{
	item->mBase = base;

	mItemList.push_back(item);

	int Y = (int)(base / 10);
	int X = base - Y * 10;
	int baseX = X - item->mItemData.X + 1;
	int baseY = Y - item->mItemData.Y + 1;
	ITEM_TYPE type = item->mItemData.mType;
	item->mIsTaken = true;

	item->SetPosition(baseX, baseY);
}

void Player::ApplyItem(Item* data)
{
	mItemAttack += data->mItemData.mMaxDamage;
	mItemArmor += data->mItemData.mArmor;
//	mItemToHit += data->mItemData.;
	// mItemHP += data->mItemData.m;
	// mItemMP += 0;
	 mItemStr += data->mItemData.mStr;
	 mItemDex += data->mItemData.mDex;
	 mItemVit += data->mItemData.mVit;
	 mItemMag += data->mItemData.mMag;

	if (data->mItemData.mType == IT_WEAPON_CLEAVER)
	{
		mHasShield = true;
	}

	std::list<Item*>::iterator iter = mItemList.begin();
	std::list<Item*>::iterator iterEnd = mItemList.end();

	for (; iter != iterEnd; )
	{
		if (data == *iter)
		{
			// (*iter)->KillThis();
			iter = mItemList.erase(iter);
			continue;
		}
		++iter;
	}
}

void Player::UnapplyItem(Item* data)
{
	mItemList.push_back(data);

	mItemAttack -= data->mItemData.mMaxDamage;
	mItemArmor -= data->mItemData.mArmor;
	
	mItemStr -= data->mItemData.mStr;
	mItemDex -= data->mItemData.mDex;
	mItemVit -= data->mItemData.mVit;
	mItemMag -= data->mItemData.mMag;
}
 
void Player::LoadEXP()
{
	if (true == mExpFile->Open())
	{
		for (size_t i = 0; i < 50; i++)
		{
			char tempArr[128] = {};
			int j = 0;
			char temp = ' ';

			while (temp != '\n')
			{
				mExpFile->Read(temp);

				tempArr[j] = temp;
				++j;
			}

			int exp = atoi(tempArr);

			mExpVec.push_back(exp);
		}
	}
}

void Player::SetMouseMonster()
{
	mMouseIndex = mColMap->PosToIndex(MyWindowClass::MainWindow()->MousePos());

	TileColRenderer* tempTile = mColMap->FindTile(mMouseIndex.X, mMouseIndex.Y);

	if (nullptr == tempTile)
	{
		mMouseMonster = nullptr;
	}
	else
	{
		mMouseMonster = tempTile->GetActor();

		if (mMouseMonster == this)
		{
			mMouseMonster = nullptr;
		}
	}

}

void Player::DebugRender()
{
	wchar_t Arr[256] = { 0, };

	swprintf_s(Arr, L"Mouse Pos : %d , %d", MyWindowClass::MainWindow()->MousePos().IntX(), MyWindowClass::MainWindow()->MousePos().IntY());
	TextOutW(MyTextureManager::BackDC(), 0, 0, Arr, lstrlen(Arr));


	swprintf_s(Arr, L"LEVEL : %d", Scene()->mLevel);
	TextOutW(MyTextureManager::BackDC(), 0, 15, Arr, lstrlen(Arr));

	if (nullptr != mMouseItem)
	{
		swprintf_s(Arr, mMouseItem->mItemData.mName.c_str());
		TextOutW(MyTextureManager::BackDC(), 0, 30, Arr, lstrlen(Arr));
	}

	if (nullptr != mWeapon)
	{
		swprintf_s(Arr, mWeapon->mItemData.mName.c_str());
		TextOutW(MyTextureManager::BackDC(), 0, 45, Arr, lstrlen(Arr));
	}

		
		/*swprintf_s(Arr, L"Player DibIndex X = %d, Y = %d", mDibTile->mX, mDibTile->mY);
	TextOutW(MyTextureManager::BackDC(), 0, 0, Arr, lstrlen(Arr));

	swprintf_s(Arr, L"Room LT = %d,%d RT = %d,%d", mDibTile->mRoom->LeftTop().IntX(), mDibTile->mRoom->LeftTop().IntY(), mDibTile->mRoom->RightTop().IntX(), mDibTile->mRoom->RightTop().IntY());
	TextOutW(MyTextureManager::BackDC(), 0, 15, Arr, lstrlen(Arr));

	swprintf_s(Arr, L"Room LB = %d,%d RB = %d,%d", mDibTile->mRoom->LeftDown().IntX(), mDibTile->mRoom->LeftDown().IntY(), mDibTile->mRoom->RightDown().IntX(), mDibTile->mRoom->RightDown().IntY());
	TextOutW(MyTextureManager::BackDC(), 0, 30, Arr, lstrlen(Arr));

	swprintf_s(Arr, L"Inven : %d Stat : %d", mIsInvenOpen, mIsStatOpen);
	TextOutW(MyTextureManager::BackDC(), 0, 45, Arr, lstrlen(Arr));

	swprintf_s(Arr, L"HP PosX : %d", mHP->SubPos().IntX());
	TextOutW(MyTextureManager::BackDC(), 0, 60, Arr, lstrlen(Arr));*/

	//// 렉탱글도 내부 비우고 선색깔도 다 바꿀수 있다.
	//Rectangle(MyTextureManager::BackDC()
	//	, CalPos().IntX() - 3
	//	, CalPos().IntY() - 3
	//	, CalPos().IntX() + 3
	//	, CalPos().IntY() + 3
	//);
}