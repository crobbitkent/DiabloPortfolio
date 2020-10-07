#pragma once

#include "Values.h"

class Room
{
public:
	MyVector2 Pos;
	MyVector2 Size;

	MyVector2 LeftTop();
	MyVector2 RightTop();
	MyVector2 LeftDown();
	MyVector2 RightDown();
};

