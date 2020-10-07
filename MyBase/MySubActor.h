#pragma once

#include "MyBaseUpdater.h"

#include "MyMath.h"

class MyActor;

// Actor�� ������ Renderer�� Collider�� ���� �� �ִ�.
class MySubActor : public MyBaseUpdater
{
public:
	MyVector2 CalPos();
	MyRect CalRect();

	void SubPos(MyVector2 subPos)
	{
		mSubPos = subPos;
	}

	void SubSize(MyVector2 subSize)
	{
		mSubSize = subSize;
	}

	MyVector2 SubPos()
	{
		return mSubPos;
	}

	MyVector2 SubSize()
	{
		return mSubSize;
	}

	MyActor* Actor() { return mActor; }

	virtual bool IsUpdated() override;
	virtual bool IsDead() override;

	void CamOnOff();

protected:
	MySubActor();
	virtual ~MySubActor();

	MyVector2 GetPos() const;

private:
	void EndColCheck();

public:

protected:
	MyActor* mActor = nullptr;
	MyVector2 mSubPos;
	MyVector2 mSubSize;
	bool mIsCam;
private:

};

