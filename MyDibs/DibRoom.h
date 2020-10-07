#pragma once

#include "Values.h"
#include <MyMath.h>

class DibRoom
{

public:
	DibRoom();
	~DibRoom();

	MyVector2 LeftTop() const;
	MyVector2 RightTop() const;
	MyVector2 LeftDown() const;
	MyVector2 RightDown() const;



public:
	MyVector2 Pos;
	MyVector2 Size;

	bool mIsStartRoom = false;
};

