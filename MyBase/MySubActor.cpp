#include "MySubActor.h"

#include "MyActor.h"



MySubActor::MySubActor()
	: mSubPos(MyVector2::ZERO)
	, mSubSize(MyVector2::ZERO)
	, mIsCam(true)
{
}


MySubActor::~MySubActor()
{
}

MyVector2 MySubActor::GetPos() const
{
	 return mActor->Pos() + mSubPos; 
}



MyVector2 MySubActor::CalPos()
{
	// 약간씩 오차가 있는것.
	if (true == mIsCam)
	{
		MyVector2 Vec = mActor->Pos() + mSubPos - (mActor->Scene()->CamPos());

		Vec.X = std::floorf(Vec.X);
		Vec.Y = std::floorf(Vec.Y);

		return Vec;
	}
	else
	{
		mSubPos.X = std::floorf(mSubPos.X);
		mSubPos.Y = std::floorf(mSubPos.Y);

		return mSubPos;
	}

	MyVector2 RVec = mActor->Pos() + mSubPos;

	RVec.X = std::floorf(RVec.X);
	RVec.Y = std::floorf(RVec.Y);

	return RVec;
}

MyRect MySubActor::CalRect()
{
	return { CalPos(), mSubSize};
}

bool MySubActor::IsUpdated()
{
	return mActor->IsUpdated() && MyBaseUpdater::IsUpdated();
}

bool MySubActor::IsDead()
{
	return mActor->IsDead() || MyBaseUpdater::IsDead();
}

void MySubActor::CamOnOff()
{
	mIsCam = !mIsCam;
}

//void MySubActor::CalIndex()
//{
//	MyVector2 pos = CalPos();
//
//	mTileIndex.X = (int)std::roundf((pos.X / QUARTER_TILE_WIDTH + pos.Y / QUARTER_TILE_HEIGHT) / 2);
//	mTileIndex.Y = (int)std::roundf((pos.Y / QUARTER_TILE_HEIGHT + -(pos.X / QUARTER_TILE_WIDTH)) / 2);
//
//}
//
//void MySubActor::CalIndex(MyVector2 newPos)
//{
//	MyVector2 pos = newPos;
//
//	mTileIndex.X = (int)std::roundf((pos.X / QUARTER_TILE_WIDTH + pos.Y / QUARTER_TILE_HEIGHT) / 2);
//	mTileIndex.Y = (int)std::roundf((pos.Y / QUARTER_TILE_HEIGHT + -(pos.X / QUARTER_TILE_WIDTH)) / 2);
//
//}
//
