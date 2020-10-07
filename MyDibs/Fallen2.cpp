#include "Fallen2.h"

#include <MyAnimationRenderer.h>
#include <MyTextureManager.h>
#include <MyInput.h>
#include <MyWindowClass.h>

#include <MyActor.h>
#include <RandomNumber.h>

#include "ColliderMap.h"
#include "Player.h"
#include "DibTile.h"
#include "TileColRenderer.h"

int Fallen2::mCount = 0;

Fallen2::Fallen2(ColliderMap* colmap, RandomMap* randomMap, Player* player, TileIndex index)
	: Monster(colmap, randomMap)
	, mPlayer(player)
	, mIsPlayerAround(true)
	, mIsExpDone(false)
{
	mVision = 5;

	mAttackSpeed = 0.05f;
	mMaxResting = 1.f;

	mData.name = L"fallen one";
	mData.dlevel = 1;
	mData.mLevel = 1;

	mData.mMaxHP = 3;
	mData.armor = 0;
	mData.toHit = 30;
	mData.attackMin = 4;
	mData.attackMax = 4;
	mData.exp = 64;

	mCurHP = mData.mMaxHP;

	SetCurPos(index.X, index.Y);

	mAnimationRenderer = CreateAniRenderer(LO_UPTILE);
	mSubRenderer = CreateAniRenderer(LO_DOWNTILE);
	mBlackRenderer = CreateRenderer<MyRenderer>(LO_BIGFOG);
	mBlackRenderer->SetRenderStyle(RS_ALPHA);


	SetAnimation();

	mAnimationRenderer->SubSize({ 128, 128 });
	mAnimationRenderer->TransColor(255, 255, 255);
	mAnimationRenderer->SubPos({ (float)0, (float)-32 });
	mAnimationRenderer->SetCurrentAnimation(L"RB_F2_IDLE");

	mSubRenderer->SubSize({ 128, 128 });
	mSubRenderer->TransColor(255, 255, 255);
	mSubRenderer->SubPos({ (float)0, (float)-32 });
	mSubRenderer->SetCurrentAnimation(L"RB_F2_IDLE");

	mBlackRenderer->SubSize({ 128, 128 });
	mBlackRenderer->TransColor(255, 255, 255);
	mBlackRenderer->SubPos({ (float)0, (float)-32 });

	mAnimationRenderer->Off();
	mSubRenderer->Off();
	mBlackRenderer->Off();

	mDir = RB;

	mIncrease = 30;
	mBase = 40;
}


Fallen2::~Fallen2()
{
}

void Fallen2::DebugRender()
{
	wchar_t Arr[256] = { 0, };

	swprintf_s(Arr, L"Fallen2 HP : %d / %d", mCurHP, mData.mMaxHP);
	TextOutW(MyTextureManager::BackDC(), 550, 0, Arr, lstrlen(Arr));

	swprintf_s(Arr, L"Fallen2 Index : X = %d , Y = %d", mTileIndex.X, mTileIndex.Y);
	TextOutW(MyTextureManager::BackDC(), 550, 15, Arr, lstrlen(Arr));

	swprintf_s(Arr, L"Fallen2 DibIndex : X = %d , Y = %d", mX, mY);
	TextOutW(MyTextureManager::BackDC(), 550, 30, Arr, lstrlen(Arr));
}

void Fallen2::Update()
{
	FindPlayer();

	if (false == CheckDir())
	{
		Idle();


		{
			std::wstring Name = L"";
			int Index = 0;

			if (true == mSubRenderer->IsOn())
			{
				Name = mSubRenderer->CurImageName();
				Index = mSubRenderer->CurFrame();
			}
			else {
				Name = mAnimationRenderer->CurImageName();
				Index = mAnimationRenderer->CurFrame();
			}

			size_t Dot = Name.find_last_of('.');
			Name = Name.replace(Dot, Name.size() - 1, L"");
			Name += L"&BLACK.bmp";
			mBlackRenderer->Image(Name.c_str(), Index);
		}

		return;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////// 안개 세팅 부분

	TileIndex tileIndex = GetTileIndex();

	TileIndex playerTileIndex = mPlayer->GetTileIndex();

	TileIndex tileDis = tileIndex - playerTileIndex;
	TileIndex lastDis;

	lastDis.X = abs(tileDis.X);
	lastDis.Y = abs(tileDis.Y);

	int gap = lastDis.X + lastDis.Y;

	gap = abs(gap);

	if (18 < gap)
	{
		mAnimationRenderer->Off();
		mSubRenderer->Off();
		mBlackRenderer->Off();
		return;
	}



	mBlackRenderer->On();

	if (1 >= gap)
	{

	}
	else if (2 == gap)
	{
		mBlackRenderer->SetBF(mBase);
	}
	else if (3 == gap)
	{
		mBlackRenderer->SetBF(mBase + mIncrease * 1);
	}
	else if (4 == gap)
	{
		mBlackRenderer->SetBF(mBase + mIncrease * 2);
	}
	else if (5 == gap)
	{
		mBlackRenderer->SetBF(mBase + mIncrease * 3);
	}
	else if (6 == gap)
	{
		mBlackRenderer->SetBF(mBase + (int)(mIncrease * 3.7));
	}
	else if (7 == gap)
	{
		mBlackRenderer->SetBF(mBase + (int)(mIncrease * 4.5));
	}
	else if (8 == gap)
	{
		mBlackRenderer->SetBF(mBase + (int)mIncrease * 5);
	}
	else if (9 == gap)
	{
		mBlackRenderer->SetBF(mBase + (int)(mIncrease * 5.5));
	}
	else if (10 == gap)
	{
		mBlackRenderer->SetBF(mBase + (int)(mIncrease * 5.8));
	}
	else if (11 == gap)
	{
		mBlackRenderer->SetBF(mBase + mIncrease * 6);
	}
	else if (12 == gap)
	{
		mBlackRenderer->SetBF(mBase + (int)(mIncrease * 6.2));
	}
	else if (13 == gap)
	{
		mBlackRenderer->SetBF(mBase + (int)(mIncrease * 6.4));
	}
	else if (14 == gap)
	{
		mBlackRenderer->SetBF(mBase + (int)(mIncrease * 6.6));
	}
	else if (15 == gap)
	{
		mBlackRenderer->SetBF(mBase + (int)(mIncrease * 6.8));
	}
	else if (16 == gap)
	{
		mBlackRenderer->SetBF(mBase + mIncrease * 7);
	}
	else
	{

	}





	///////////////////////////////////////////////////////////////////////////////////////////////////

	if (false == mIsDying)
	{
		// 플레이어보다 스켈레톤이 더 오른쪽에 있으면 또는 더 뒤에 있으면
		// 스켈레톤이 먼저 출력
		bool renderOrder = (mPlayer->GetTileIndex().X < GetTileIndex().X || mPlayer->GetTileIndex().Y < GetTileIndex().Y);

		if (true == renderOrder)
		{
			mAnimationRenderer->Off();
		}
		else
		{
			mAnimationRenderer->On();
		}

		if (false == renderOrder)
		{
			mSubRenderer->Off();
		}
		else
		{
			mSubRenderer->On();
		}
	}
	else
	{
		Die();


		{
			std::wstring Name = L"";
			int Index = 0;

			if (true == mSubRenderer->IsOn())
			{
				Name = mSubRenderer->CurImageName();
				Index = mSubRenderer->CurFrame();
			}
			else {
				Name = mAnimationRenderer->CurImageName();
				Index = mAnimationRenderer->CurFrame();
			}

			size_t Dot = Name.find_last_of('.');
			Name = Name.replace(Dot, Name.size() - 1, L"");
			Name += L"&BLACK.bmp";
			mBlackRenderer->Image(Name.c_str(), Index);
		}

		return;
	}

	CheckDirChange();
	SetUnit();

	StateUpdate();

	switch (mState)
	{
	case MS_IDLE:
		Idle();
		break;
	case MS_WALK:
		Walk();
		break;
	case MS_ATTACK:
		Attack();
		break;
	case MS_DIE:
		Die();
		break;
	case MS_GETHIT:
		GetHit();
		break;
	case MS_SPECIAL:
		Special();
		break;
	case MS_REST:
		Rest();
		break;
	case MS_MAX:
		BOOM;
		break;
	default:
		break;
	}

	{
		std::wstring Name = L"";
		int Index = 0;

		if (true == mSubRenderer->IsOn())
		{
			Name = mSubRenderer->CurImageName();
			Index = mSubRenderer->CurFrame();
		}
		else {
			Name = mAnimationRenderer->CurImageName();
			Index = mAnimationRenderer->CurFrame();
		}

		size_t Dot = Name.find_last_of('.');
		Name = Name.replace(Dot, Name.size() - 1, L"");
		Name += L"&BLACK.bmp";
		mBlackRenderer->Image(Name.c_str(), Index);
	}

}

void Fallen2::StateUpdate()
{
	// 죽음
	if (0 >= mCurHP && false == mIsDead)
	{
		SetCurrentState(MS_DIE);
		mIsDying = true;

		CalIndex();

		mColMap->UnsetUnit(mTileIndex.X, mTileIndex.Y);
		++mCount;
		Monster::mKillCount = mCount;
		ResetPosM(this);
	}
}

void Fallen2::FindPlayer()
{
	TileIndex tileIndex = GetTileIndex();
	TileIndex playerTileIndex = mPlayer->GetTileIndex();

	TileIndex tileDis = tileIndex - playerTileIndex;
	TileIndex lastDis;

	lastDis.X = abs(tileDis.X);
	lastDis.Y = abs(tileDis.Y);

	if (mVision >= lastDis.X && mVision >= lastDis.Y)
	{
		mIsPlayerAround = true;

		return;
	}

	mIsPlayerAround = false;
}

void Fallen2::CheckDirChange()
{
	if (mDir != mPrevDir && false == mIsAttacking)
	{
		if (MS_REST != mState)
		{
			SetCurrentState(mState);
		}

		mPrevDir = mDir;
	}
}

bool Fallen2::CheckDir()
{
	if (mCurDestination == MyVector2::ZERO)
	{
		mDir = RB;

		return true;
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

	int a = 0;

	// 도착해서 옆에 몬스터가 있고 때리려고 하기 바로 전

	if (0 == mPathList.size() && true == mGoingToAttack)
	{
		TileColRenderer* temp = mDestDibTile;

		Monster::SetDir(mDestDibTile);

		MONSTER_DIR tempDir = mDir;

		if (nullptr != mDestDibTile)
		{
			mMonster = mDestDibTile->GetActor();
		}
	}

	return true;
}

void Fallen2::Idle()
{
	// Rest 상태라면
	if (true == mIsResting)
	{
		mIsAttacking = false;
		Rest();
	}
	// 아니라면
	else if (true == mIsPlayerAround)
	{
		MyVector2 playerPos = mPlayer->CalPos();

		GoingToAttack(playerPos);
	}
}



void Fallen2::Walk()
{
	MyVector2 temp = MyMath::Lerp(mStart, mCurDestination, MyTime::DeltaTime());

	mIsMoving = true;

	Move(temp * mMoveSpeed);

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

	TileColRenderer* nextTile = mColMap->GetTile(mCurDestination - Scene()->CamPos());

	if (this != nextTile->GetActor())
	{
		mPathList.clear();
		Pos(mStart);
		Rest();
	}
}


// 한칸 마다 할 일들
void Fallen2::WalkTask()
{
	// 타일에 도착하면 먼저 deltaTime때문에 오차가 생겼는데 이걸을 없애준다.
	Pos(mCurDestination);
	// 카메라 위치도 오차 없애기
	// Scene()->CamPos(Pos() + MyVector2(-400.f, -300.f));
	mStart = Pos();

	// 경우 1
	// 이제 도착! => Idle
	if (0 == mPathList.size())
	{
		// 경우 1-2 도착했는데 때리고 싶은 경우!!!!
		if (true == mGoingToAttack)
		{
			// 경우 1-2-1 때리자...
			// 그 전에 dest 갱신
			TileIndex destIndex = mPlayer->GetTileIndex();
			// 그 타일 인덱스의 타일
			TileColRenderer* temp = mColMap->GetTile(destIndex.X, destIndex.Y);


			if (nullptr != mDestDibTile->GetActor() || this != mDestDibTile->GetActor())
			{
				if (temp->GetTileIndex() == mDestDibTile->GetTileIndex())
				{
					SetDir(temp);

					int ran = RandomNumber::GetRandomNumber(1, 2);

					if (1 == ran)
					{
						SetCurrentState(MS_ATTACK);
					}
					else
					{
						SetCurrentState(MS_SPECIAL);
					}

					mGoingToAttack = false;

					return;
				}

			}
			// 경우 1-2-2 없으면 PS_IDLE		
		}

		Rest();
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
		TileColRenderer* nextTile = mColMap->GetTile(mCurDestination - Scene()->CamPos());

		if (nullptr == nextTile)
		{
			Rest();
			mIsMoving = false;
			return;
		}

		if (nullptr != nextTile->GetActor() && this != nextTile->GetActor())
		{
			Rest();
			mIsMoving = false;
			return;
		}

		// 방향 업데이트
		SetDir(mCurDestination);
		// -------------------------------------------------------------------------------------------

		// 미리 한칸을 움직여서 겹치는 것을 방지!
		PreMove();
	}
}

void Fallen2::Attack()
{
	// 첫 공격시
	if (false == mIsAttacking)
	{
		if (nullptr == mDestDibTile)
		{
			Rest();
		}

		if (nullptr == mDestDibTile->GetActor())
		{
			Rest();
		}
	}


	if (true == mAnimationRenderer->IsCurrentAnimationEnd())
	{
		mIsAttacking = false;
		mGoingToAttack = false;
		mAttacked = false;
		mMonster = nullptr;
		SetCurrentState(MS_IDLE);
		mPathList.clear();

		mSubRenderer->FinishAnimation();
		 

		Rest();
	}
	else if (true == mSubRenderer->IsCurrentAnimationEnd())
	{
		mIsAttacking = false;
		mGoingToAttack = false;
		mAttacked = false;
		mMonster = nullptr;
		SetCurrentState(MS_IDLE);
		mPathList.clear();

		mAnimationRenderer->FinishAnimation();
		 

		Rest();
	}
	else
	{
		// 방향 구하기...
		// SetDir(mDestDibTile);

		// 방향 바뀐 것 적용
		CheckDirChange();
		int ran = RandomNumber::GetRandomNumber(1, 100);
		mIsAttacking = true;

		if (0 != mAnimationRenderer->CurFrame() - mDir * 13 &&
			9 == (mAnimationRenderer->CurFrame() - mDir * 13) &&
			false == mAttacked)
		{
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
				if (ran <= mData.toHit)
				{
					mMonster->GetDamage(mData.attackMin);
				}
			}
		}
		else if (0 != mSubRenderer->CurFrame() - mDir * 13 &&
			9 == (mSubRenderer->CurFrame() - mDir * 13) &&
			false == mAttacked)
		{
			int a = mSubRenderer->CurFrame();

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
				if (ran <= mData.toHit)
				{
					mMonster->GetDamage(mData.attackMin);
				}
			}
		}
	}
}

void Fallen2::Die()
{
	if (true == mAnimationRenderer->IsCurrentAnimationEnd())
	{
		mIsDying = false;
		mIsDead = true;
		mSubRenderer->FinishAnimation();
		 
	}
	else if (true == mSubRenderer->IsCurrentAnimationEnd())
	{
		mIsDying = false;
		mIsDead = true;
		mAnimationRenderer->FinishAnimation();
		 
	}
	// SetCurrentState(F_DIE);
	if (false == mIsExpDone)
	{
		mPlayer->GetExp(mData.exp);
		mIsExpDone = true;
	}

	if (false == mIsDropped)
	{
		NormalDrop();
		mIsDropped = true;
	}
}

void Fallen2::GetHit()
{

	if (true == mAnimationRenderer->IsCurrentAnimationEnd() || true == mSubRenderer->IsCurrentAnimationEnd())
	{
		SetCurrentState(MS_IDLE);
	}

	Pos(mStart);
}

void Fallen2::Special()
{
	// 첫 공격시
	if (false == mIsAttacking)
	{
		if (nullptr == mDestDibTile)
		{
			Rest();
		}

		if (nullptr == mDestDibTile->GetActor())
		{
			Rest();
		}
	}


	if (true == mAnimationRenderer->IsCurrentAnimationEnd())
	{
		mIsAttacking = false;
		mGoingToAttack = false;
		mAttacked = false;
		mMonster = nullptr;
		SetCurrentState(MS_IDLE);
		mPathList.clear();

		mSubRenderer->FinishAnimation();
		 

		Rest();
	}
	else if (true == mSubRenderer->IsCurrentAnimationEnd())
	{
		mIsAttacking = false;
		mGoingToAttack = false;
		mAttacked = false;
		mMonster = nullptr;
		SetCurrentState(MS_IDLE);
		mPathList.clear();

		mAnimationRenderer->FinishAnimation();
		 

		Rest();
	}
	else
	{
		// 방향 구하기...
		// SetDir(mDestDibTile);

		// 방향 바뀐 것 적용
		CheckDirChange();
		int ran = RandomNumber::GetRandomNumber(1, 100);
		mIsAttacking = true;

		if (0 != mAnimationRenderer->CurFrame() - mDir * 15 &&
			12 == (mAnimationRenderer->CurFrame() - mDir * 15) &&
			false == mAttacked)
		{
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
				if (ran <= mData.toHit * 2)
				{
					mMonster->GetDamage(mData.attackMin * 2);
				}
			}
		}
		else if (0 != mSubRenderer->CurFrame() - mDir * 15 &&
			12 == (mSubRenderer->CurFrame() - mDir * 15) &&
			false == mAttacked)
		{
			int a = mSubRenderer->CurFrame();

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
				if (ran <= mData.toHit * 2)
				{
					mMonster->GetDamage(mData.attackMin * 2);
				}
			}
		}
	}
}

void Fallen2::Rest()
{
	mIsAttacking = false;

	mState = MS_REST;

	// 첫 휴식
	if (false == mIsResting)
	{
		std::wstring newState = mAllDirName[mDir];

		newState.append(L"_");
		newState.append(L"F2_IDLE");

		mAnimationRenderer->SetCurrentAnimation(newState.c_str());
		mSubRenderer->SetCurrentAnimation(newState.c_str());
	}



	mIsResting = true;

	mRestingCount += MyTime::DeltaTime();

	mPathList.clear();

	if (mMaxResting <= mRestingCount)
	{
		mRestingCount = 0.0f;
		mIsResting = false;
		SetCurrentState(MS_IDLE);
	}
}

void Fallen2::SetCurrentState(MONSTER_STATE state, bool noneDir /*= false*/)
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
	mSubRenderer->SetCurrentAnimation(newState.c_str());
}

void Fallen2::GetDamage(int damage)
{
	mCurHP -= damage;

	mIsAttacking = false;

	SetCurrentState(MS_GETHIT);

	Monster::ResetPosM(this);
}

void Fallen2::SetAnimation()
{
	// SetAnimation
	mAllDirName[T] = L"T";
	mAllDirName[LT] = L"LT";
	mAllDirName[L] = L"L";
	mAllDirName[LB] = L"LB";
	mAllDirName[B] = L"B";
	mAllDirName[RB] = L"RB";
	mAllDirName[R] = L"R";
	mAllDirName[RT] = L"RT";

	mAllStateName[MS_IDLE] = L"F2_IDLE";
	mAllStateName[MS_WALK] = L"F2_WALK";
	mAllStateName[MS_ATTACK] = L"F2_ATTACK";
	mAllStateName[MS_DIE] = L"F2_DIE";
	mAllStateName[MS_GETHIT] = L"F2_GETHIT";
	mAllStateName[MS_SPECIAL] = L"F2_SPECIAL";

	AutoAnimation(L"ATTACK", 12, mAttackSpeed);
	AutoAnimation(L"DIE", 13, false, 0.05f);
	AutoAnimation(L"GETHIT", 10, 0.05f);
	AutoAnimation(L"IDLE", 11, 0.1f);
	AutoAnimation(L"SPECIAL", 14);
	AutoAnimation(L"WALK", 11);
}

// 가로 인덱스
void Fallen2::AutoAnimation(const wchar_t * name, int index, float frameSpeed)
{
	// mAnimationRenderer->CreateAnimation(L"RB_F2_IDLE", L"F&IDLE.bmp", 0, 12, true);
	// mAnimationRenderer->CreateAnimation(L"RB_F2_GETHIT", L"F&GETHIT.bmp", 0, 7, true);

	int start = 0;
	int next = index;

	std::wstring fileName = L"F2&";
	fileName.append(name);
	fileName.append(L".bmp");

	std::wstring blackName = L"F2&";
	blackName.append(name);
	blackName.append(L"&BLACK.bmp");

	for (int i = 0; i < 8; ++i)
	{
		std::wstring aniName = mAllDirName[i];

		aniName.append(L"_F2_");
		aniName.append(name);

		mAnimationRenderer->CreateAnimation(aniName.c_str(), fileName.c_str(), start, next, true, frameSpeed);
		mSubRenderer->CreateAnimation(aniName.c_str(), fileName.c_str(), start, next, true, frameSpeed);

		start = next + 1;
		next = start + index;

		/////////////////////////////////////////////////////////////////
	}
}

void Fallen2::AutoAnimation(const wchar_t * name, int index, bool loop, float frameSpeed)
{

	int start = 0;
	int next = index;

	std::wstring fileName = L"F2&";
	fileName.append(name);
	fileName.append(L".bmp");

	std::wstring blackName = L"F2&";
	blackName.append(name);
	blackName.append(L"&BLACK.bmp");

	for (int i = 0; i < 8; ++i)
	{
		std::wstring aniName = mAllDirName[i];

		aniName.append(L"_F2_");
		aniName.append(name);

		mAnimationRenderer->CreateAnimation(aniName.c_str(), fileName.c_str(), start, next, loop, frameSpeed);
		mSubRenderer->CreateAnimation(aniName.c_str(), fileName.c_str(), start, next, loop, frameSpeed);

		start = next + 1;
		next = start + index;
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------------------

// 1. mCurDestination => 나의 다음 타일을 설정한다.
// 2. 다음 타일을 미리 점유한다.(다른 캐릭과 겹치기 방지)
// 예상 문제 -> 이동하려고 하면 다음 타일을 미리 점유하지만 맞으면 제잘
// 이를 해결하기 위해서 맞으면 점유한 타일을 모두 Reset한다. => ResetPos();
void Fallen2::GoingToMove(MyVector2 mousePos)
{
	mPathList.clear();
	// 길찾기
	mPathList = mColMap->PathFindM(CalPos(), mousePos);

	// 길을 못 찾음
	if (0 == mPathList.size())
	{
		SetCurrentState(MS_IDLE);

		return;
	}

	// 현재 타일은 제거...
	mPathList.pop_front();

	// 나의 다음 타일
	mCurDestination = (*mPathList.begin());
	// 나의 다음 타일 사용했으니 또 제거
	mPathList.pop_front();

	SetDir(mCurDestination);
	// 걷기
	SetCurrentState(MS_WALK);

	// 미리 한칸을 움직여서 겹치는 것을 방지!
	PreMove();
}

// 찍은 곳이 몬스터가 있는곳.
// 마지막 타일을 pop_back한다.
void Fallen2::GoingToAttack(MyVector2 mousePos)
{
	mPathList.clear();
	// 길찾기
	mPathList = mColMap->PathFindM(CalPos(), mousePos);

	// 길을 못 찾음
	if (0 == mPathList.size())
	{
		SetCurrentState(MS_IDLE);

		return;
	}

	// 몹이 바로 앞!!!
	if (2 == mPathList.size())
	{
		SetDir((*--mPathList.end()));

		mIsAttacking = true;

		int ran = RandomNumber::GetRandomNumber(1, 2);

		if (1 == ran)
		{
			SetCurrentState(MS_ATTACK);
		}
		else
		{
			SetCurrentState(MS_SPECIAL);
		}

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
	SetCurrentState(MS_WALK);

	// 미리 한칸을 움직여서 겹치는 것을 방지!
	PreMove();
}

// 미리 한칸을 움직여서 겹치는 것을 방지!
void Fallen2::PreMove()
{
	MyVector2 first = mCurDestination;
	first -= CamPos();
	TileColRenderer* firstTile = mColMap->GetTile(first.X, first.Y);
	firstTile->SetActor(this);
}
