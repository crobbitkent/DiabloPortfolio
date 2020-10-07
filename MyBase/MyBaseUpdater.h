#pragma once

#include "MyMacro.h"

// Renderer�� ���� �ѱ� �ϱ� ����
class MyBaseUpdater
{
public:
	MyBaseUpdater();
	// ���� / �Ҹ� �Ұ�
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

