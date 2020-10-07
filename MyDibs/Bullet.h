#pragma once

#include <MyMacro.h>

#include <MyActor.h>

class Bullet : public MyActor
{
public:
	Bullet();
	Bullet(MyVector2 cursor);
	~Bullet();

	void Update() override;
	void DebugRender() override;

public:
	float Time;

private:
	MyVector2 mDir;
};

