#include "Bullet.h"

#include <MyScene.h>
#include <MyInput.h>
#include <MyTime.h>
#include <MyRenderer.h>
#include <MyMacro.h>

Bullet::Bullet()
	: Time(0.0f)
{
	//MyRenderer* newRender = CreateRenderer<MyRenderer>(LO_BULLET);
	//newRender->SubSize({ 30, 30 });
}

Bullet::Bullet(MyVector2 cursor)
	: Time(0.0f)
	, mDir(cursor)
{
	//MyRenderer* newRender = CreateRenderer<MyRenderer>(LO_BULLET);
	//newRender->SubSize({ 30, 30 });

}

Bullet::~Bullet()
{
}

void Bullet::Update()
{
	Time += MyTime::DeltaTime();

	if (Time >= 1)
	{
		KillThis();
	}

	// 방향 구하기
	Move(mDir * MyTime::DeltaTime() * 500.f);
}

void Bullet::DebugRender()
{
}
