#include "Scavenger.h"

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

int Scavenger::mCount = 0;

Scavenger::Scavenger(ColliderMap * colmap, RandomMap * randomMap, Player * player)
	: Monster(colmap, randomMap)
	, mPlayer(player)
	, mIsPlayerAround(true)
	, mIsExpDone(false)
{
	mVision = 5;

	mAttackSpeed = 0.05f;
	mMaxResting = 1.f;

	mData.name = L"scavenger";
	mData.dlevel = 1;
	mData.mLevel = 1;

	mData.mMaxHP = 3;
	mData.armor = 0;
	mData.toHit = 30;
	mData.attackMin = 4;
	mData.attackMax = 4;
	mData.exp = 64;

	mCurHP = mData.mMaxHP;

	mAnimationRenderer = CreateAniRenderer(LO_UPTILE);
	mSubRenderer = CreateAniRenderer(LO_DOWNTILE);
	mBlackRenderer = CreateRenderer<MyRenderer>(LO_BIGFOG);
	mBlackRenderer->SetRenderStyle(RS_ALPHA);

	SetAnimation();

	mAnimationRenderer->SubSize({ 128, 128 });
	mAnimationRenderer->TransColor(255, 255, 255);
	mAnimationRenderer->SubPos({ (float)0, (float)-32 });
	mAnimationRenderer->SetCurrentAnimation(L"RB_SC_IDLE");

	mSubRenderer->SubSize({ 128, 128 });
	mSubRenderer->TransColor(255, 255, 255);
	mSubRenderer->SubPos({ (float)0, (float)-32 });
	mSubRenderer->SetCurrentAnimation(L"RB_SC_IDLE");

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

Scavenger::Scavenger(ColliderMap* colmap, RandomMap* randomMap, Player* player, TileIndex index)
	: Monster(colmap, randomMap)
	, mPlayer(player)
	, mIsPlayerAround(true)
	, mIsExpDone(false)
{
	mVision = 5;

	mAttackSpeed = 0.05f;
	mMaxResting = 1.f;

	mData.name = L"scavenger";
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
	mAnimationRenderer->SetCurrentAnimation(L"RB_SC_IDLE");

	mSubRenderer->SubSize({ 128, 128 });
	mSubRenderer->TransColor(255, 255, 255);
	mSubRenderer->SubPos({ (float)0, (float)-32 });
	mSubRenderer->SetCurrentAnimation(L"RB_SC_IDLE");

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


Scavenger::~Scavenger()
{
}

void Scavenger::DebugRender()
{
	wchar_t Arr[256] = { 0, };

	swprintf_s(Arr, L"Scavenger HP : %d / %d", mCurHP, mData.mMaxHP);
	TextOutW(MyTextureManager::BackDC(), 550, 0, Arr, lstrlen(Arr));

	swprintf_s(Arr, L"Scavenger Index : X = %d , Y = %d", mTileIndex.X, mTileIndex.Y);
	TextOutW(MyTextureManager::BackDC(), 550, 15, Arr, lstrlen(Arr));

	swprintf_s(Arr, L"Scavenger DibIndex : X = %d , Y = %d", mX, mY);
	TextOutW(MyTextureManager::BackDC(), 550, 30, Arr, lstrlen(Arr));
}

void Scavenger::Update()
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

	/////////////////////////////////////////////////////////////////////////////////////////////////// �Ȱ� ���� �κ�

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
		// �÷��̾�� ���̷����� �� �����ʿ� ������ �Ǵ� �� �ڿ� ������
		// ���̷����� ���� ���
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

void Scavenger::StateUpdate()
{
	// ����
	if (0 >= mCurHP && false == mIsDead)
	{
		int rand = RandomNumber::GetRandomNumber(1, 2);

		if (1 == rand)
		{
			MyGameSound::SoundPlay(L"scavd1.wav");
		}
		else
		{
			MyGameSound::SoundPlay(L"scavd2.wav");
		}

		SetCurrentState(MS_DIE);
		mIsDying = true;

		CalIndex();

		mColMap->UnsetUnit(mTileIndex.X, mTileIndex.Y);
		++mCount;
		Monster::mKillCount = mCount;
		ResetPosM(this);
	}
}

void Scavenger::FindPlayer()
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

void Scavenger::CheckDirChange()
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

bool Scavenger::CheckDir()
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

	// �����ؼ� ���� ���Ͱ� �ְ� �������� �ϱ� �ٷ� ��

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

void Scavenger::Idle()
{
	// Rest ���¶��
	if (true == mIsResting)
	{
		mIsAttacking = false;
		Rest();
	}
	// �ƴ϶��
	else if (true == mIsPlayerAround)
	{
		MyVector2 playerPos = mPlayer->CalPos();

		GoingToAttack(playerPos);
	}
}



void Scavenger::Walk()
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


// ��ĭ ���� �� �ϵ�
void Scavenger::WalkTask()
{
	// Ÿ�Ͽ� �����ϸ� ���� deltaTime������ ������ ����µ� �̰��� �����ش�.
	Pos(mCurDestination);
	// ī�޶� ��ġ�� ���� ���ֱ�
	// Scene()->CamPos(Pos() + MyVector2(-400.f, -300.f));
	mStart = Pos();

	// ��� 1
	// ���� ����! => Idle
	if (0 == mPathList.size())
	{
		// ��� 1-2 �����ߴµ� ������ ���� ���!!!!
		if (true == mGoingToAttack)
		{
			// ��� 1-2-1 ������...
			// �� ���� dest ����
			TileIndex destIndex = mPlayer->GetTileIndex();
			// �� Ÿ�� �ε����� Ÿ��
			TileColRenderer* temp = mColMap->GetTile(destIndex.X, destIndex.Y);


			if (nullptr != mDestDibTile->GetActor() || this != mDestDibTile->GetActor())
			{
				if (temp->GetTileIndex() == mDestDibTile->GetTileIndex())
				{
					SetDir(temp);

					SetCurrentState(MS_ATTACK);

					mGoingToAttack = false;

					return;
				}

			}
			// ��� 1-2-2 ������ PS_IDLE		
		}

		Rest();
	}
	// ��� 2
	// ���� ������ �ִ�!
	else
	{
		// ã�Ƴ��� �濡�� ���� Ÿ���� ���� �ش�.
		// mCurDestination�� ������Ʈ!
		mCurDestination = (*mPathList.begin());
		mPathList.pop_front();

		// -------------------------------------------------------------------------------------------
		// �� �濡 ���� �ִ��� �ٽ� Ȯ��!!!
		// ���� ������ �����.
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

		// ���� ������Ʈ
		SetDir(mCurDestination);
		// -------------------------------------------------------------------------------------------

		// �̸� ��ĭ�� �������� ��ġ�� ���� ����!
		PreMove();
	}
}

void Scavenger::Attack()
{
	// ù ���ݽ�
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
		// ���� ���ϱ�...
		// SetDir(mDestDibTile);

		// ���� �ٲ� �� ����
		CheckDirChange();
		int ran = RandomNumber::GetRandomNumber(1, 100);
		mIsAttacking = true;

		if (0 != mAnimationRenderer->CurFrame() - mDir * 12 &&
			8 == (mAnimationRenderer->CurFrame() - mDir * 12) &&
			false == mAttacked)
		{
			int a = mAnimationRenderer->CurFrame();

			int rand = RandomNumber::GetRandomNumber(1, 2);

			if (1 == rand)
			{
				MyGameSound::SoundPlay(L"scava1.wav");
			}
			else
			{
				MyGameSound::SoundPlay(L"scava2.wav");
			}

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
		else if (0 != mSubRenderer->CurFrame() - mDir * 12 &&
			8 == (mSubRenderer->CurFrame() - mDir * 12) &&
			false == mAttacked)
		{
			int a = mSubRenderer->CurFrame();

			int rand = RandomNumber::GetRandomNumber(1, 2);

			if (1 == rand)
			{
				MyGameSound::SoundPlay(L"scava1.wav");
			}
			else
			{
				MyGameSound::SoundPlay(L"scava2.wav");
			}

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

void Scavenger::Die()
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

void Scavenger::GetHit()
{

	if (true == mAnimationRenderer->IsCurrentAnimationEnd() || true == mSubRenderer->IsCurrentAnimationEnd())
	{
		SetCurrentState(MS_IDLE);
	}

	Pos(mStart);
}

void Scavenger::Special()
{
}

void Scavenger::Rest()
{
	mIsAttacking = false;

	mState = MS_REST;

	// ù �޽�
	if (false == mIsResting)
	{
		std::wstring newState = mAllDirName[mDir];

		newState.append(L"_");
		newState.append(L"SC_IDLE");

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

void Scavenger::SetCurrentState(MONSTER_STATE state, bool noneDir /*= false*/)
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

void Scavenger::GetDamage(int damage)
{
	int rand = RandomNumber::GetRandomNumber(1, 2);

	if (1 == rand)
	{
		MyGameSound::SoundPlay(L"scavh1.wav");
	}
	else
	{
		MyGameSound::SoundPlay(L"scavh2.wav");
	}

	mCurHP -= damage;

	mIsAttacking = false;

	SetCurrentState(MS_GETHIT);

	Monster::ResetPosM(this);
}

void Scavenger::SetAnimation()
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

	mAllStateName[MS_IDLE] = L"SC_IDLE";
	mAllStateName[MS_WALK] = L"SC_WALK";
	mAllStateName[MS_ATTACK] = L"SC_ATTACK";
	mAllStateName[MS_DIE] = L"SC_DIE";
	mAllStateName[MS_GETHIT] = L"SC_GETHIT";

	AutoAnimation(L"ATTACK", 11, mAttackSpeed);
	AutoAnimation(L"DIE", 19, false, 0.05f);
	AutoAnimation(L"GETHIT", 7, 0.05f);
	AutoAnimation(L"IDLE", 11, 0.1f);
	AutoAnimation(L"WALK", 7);
}

// ���� �ε���
void Scavenger::AutoAnimation(const wchar_t * name, int index, float frameSpeed)
{
	// mAnimationRenderer->CreateAnimation(L"RB_SC_IDLE", L"F&IDLE.bmp", 0, 12, true);
	// mAnimationRenderer->CreateAnimation(L"RB_SC_GETHIT", L"F&GETHIT.bmp", 0, 7, true);

	int start = 0;
	int next = index;

	std::wstring fileName = L"SC&";
	fileName.append(name);
	fileName.append(L".bmp");

	std::wstring blackName = L"SC&";
	blackName.append(name);
	blackName.append(L"&BLACK.bmp");

	for (int i = 0; i < 8; ++i)
	{
		std::wstring aniName = mAllDirName[i];

		aniName.append(L"_SC_");
		aniName.append(name);

		mAnimationRenderer->CreateAnimation(aniName.c_str(), fileName.c_str(), start, next, true, frameSpeed);
		mSubRenderer->CreateAnimation(aniName.c_str(), fileName.c_str(), start, next, true, frameSpeed);

		start = next + 1;
		next = start + index;

		/////////////////////////////////////////////////////////////////
	}
}

void Scavenger::AutoAnimation(const wchar_t * name, int index, bool loop, float frameSpeed)
{

	int start = 0;
	int next = index;

	std::wstring fileName = L"SC&";
	fileName.append(name);
	fileName.append(L".bmp");

	std::wstring blackName = L"SC&";
	blackName.append(name);
	blackName.append(L"&BLACK.bmp");

	for (int i = 0; i < 8; ++i)
	{
		std::wstring aniName = mAllDirName[i];

		aniName.append(L"_SC_");
		aniName.append(name);

		mAnimationRenderer->CreateAnimation(aniName.c_str(), fileName.c_str(), start, next, loop, frameSpeed);
		mSubRenderer->CreateAnimation(aniName.c_str(), fileName.c_str(), start, next, loop, frameSpeed);

		start = next + 1;
		next = start + index;
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------------------

// 1. mCurDestination => ���� ���� Ÿ���� �����Ѵ�.
// 2. ���� Ÿ���� �̸� �����Ѵ�.(�ٸ� ĳ���� ��ġ�� ����)
// ���� ���� -> �̵��Ϸ��� �ϸ� ���� Ÿ���� �̸� ���������� ������ ����
// �̸� �ذ��ϱ� ���ؼ� ������ ������ Ÿ���� ��� Reset�Ѵ�. => ResetPos();
void Scavenger::GoingToMove(MyVector2 mousePos)
{
	mPathList.clear();
	// ��ã��
	mPathList = mColMap->PathFindM(CalPos(), mousePos);

	// ���� �� ã��
	if (0 == mPathList.size())
	{
		SetCurrentState(MS_IDLE);

		return;
	}

	// ���� Ÿ���� ����...
	mPathList.pop_front();

	// ���� ���� Ÿ��
	mCurDestination = (*mPathList.begin());
	// ���� ���� Ÿ�� ��������� �� ����
	mPathList.pop_front();

	SetDir(mCurDestination);
	// �ȱ�
	SetCurrentState(MS_WALK);

	// �̸� ��ĭ�� �������� ��ġ�� ���� ����!
	PreMove();
}

// ���� ���� ���Ͱ� �ִ°�.
// ������ Ÿ���� pop_back�Ѵ�.
void Scavenger::GoingToAttack(MyVector2 mousePos)
{
	mPathList.clear();
	// ��ã��
	mPathList = mColMap->PathFindM(CalPos(), mousePos);

	// ���� �� ã��
	if (0 == mPathList.size())
	{
		SetCurrentState(MS_IDLE);

		return;
	}

	// ���� �ٷ� ��!!!
	if (2 == mPathList.size())
	{
		SetDir((*--mPathList.end()));

		mIsAttacking = true;

		SetCurrentState(MS_ATTACK);

		return;
	}

	// ���� Ÿ���� ����...
	mPathList.pop_front();

	// ���� ���� Ÿ��
	mCurDestination = (*mPathList.begin());
	// ���� ���� Ÿ�� ��������� �� ����
	mPathList.pop_front();

	//-----------------------------------------------------------------------------------------

	// GoingToMove�� �ٸ� ��!!! ������ ��ġ = ���Ͱ� ����
	// ���� �������� ������ �� Ÿ���� �ȴ�. 
	// ���콺 ��ġ�� Ÿ�� �ε���
	TileIndex destIndex = mColMap->PosToIndex(mousePos);
	// �� Ÿ�� �ε����� Ÿ��
	mDestDibTile = mColMap->GetTile(destIndex.X, destIndex.Y);

	// ���� Ÿ�� ����
	mPathList.pop_back();

	// �������� ���� �����ڴٴ� ��
	mGoingToAttack = true;

	//-----------------------------------------------------------------------------------------

	// �ȱ�
	SetDir(mCurDestination);
	SetCurrentState(MS_WALK);

	// �̸� ��ĭ�� �������� ��ġ�� ���� ����!
	PreMove();
}

// �̸� ��ĭ�� �������� ��ġ�� ���� ����!
void Scavenger::PreMove()
{
	MyVector2 first = mCurDestination;
	first -= CamPos();
	TileColRenderer* firstTile = mColMap->GetTile(first.X, first.Y);
	firstTile->SetActor(this);
}
