#pragma once

#include "MyMacro.h"

// Renderer를 껐다 켜기 하기 위함
class MyBaseUpdater
{
public:
	MyBaseUpdater();
	// 생성 / 소멸 불가
	virtual ~MyBaseUpdater() = 0;
	
	bool IsOff() { return false == mIsUpdated; }
	bool IsOn() { return true == mIsUpdated; }
	void On() { mIsUpdated = true; }
	virtual void Off() { mIsUpdated = false; }
	void KillThis() { mIsDead = true; }

	virtual bool IsUpdated() { return (false == mIsDead) && (true == mIsUpdated); }
	virtual bool IsDead() { return mIsDead; }


	TileIndex GetTileIndex() { return mTileIndex; }
	void SetTileIndex(TileIndex index)
	{
		mTileIndex.X = index.X;
		mTileIndex.Y = index.Y;
	}

protected:




protected:
	TileIndex mTileIndex;

private:
	bool mIsUpdated;
	bool mIsDead;


};

