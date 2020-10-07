#pragma once

#include <MyWindowClass.h>

class MyClientUpdater :	public MyWindowClass::LoopUpdater
{
public:
	MyClientUpdater();
	~MyClientUpdater();
	virtual void GameInit() override;
	virtual void GameUpdate() override;

public:
	MyRect Rect;

};

