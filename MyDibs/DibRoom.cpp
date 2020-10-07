#include "DibRoom.h"

#include <MyMath.h>

DibRoom::DibRoom()
{
}


DibRoom::~DibRoom()
{
}

MyVector2 DibRoom::LeftTop() const
{
	return Pos;
}

MyVector2 DibRoom::RightTop() const
{


	return Pos + MyVector2(Size.X + 2, 0.f);
}

MyVector2 DibRoom::LeftDown() const 
{
	return Pos + MyVector2(0.f, Size.Y + 2);
}

MyVector2 DibRoom::RightDown() const
{
	return Pos + MyVector2(Size.X + 2, Size.Y + 2);
}
