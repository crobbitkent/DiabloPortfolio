#include "MyMath.h"
#include "MyTime.h"

MyVector2 MyVector2::DLeft;
MyVector2 MyVector2::DRight;
MyVector2 MyVector2::DTop;
MyVector2 MyVector2::DBottom;

const MyVector2 MyVector2::ZERO = { 0,  0 };
const MyVector2 MyVector2::LEFT = { -1,  0 };
const MyVector2 MyVector2::LEFTDOWN = { -1,  1 };
const MyVector2 MyVector2::DOWN = { 0 ,  1 };
const MyVector2 MyVector2::DOWNRIGHT = { 1 ,  1 };
const MyVector2 MyVector2::RIGHT = { 1 ,  0 };
const MyVector2 MyVector2::RIGHTUP = { 1 , -1 };
const MyVector2 MyVector2::UP = { 0 , -1 };
const MyVector2 MyVector2::UPLEFT = { -1, -1 };

const MyVector2 MyVector2::ARRDIR[8] = { MyVector2::LEFT, MyVector2::LEFTDOWN, MyVector2::DOWN, MyVector2::DOWNRIGHT
									 , MyVector2::RIGHT, MyVector2::RIGHTUP, MyVector2::UP, MyVector2::UPLEFT };

MyVector2 MyVector2::DeltaDirArr[MD_MAX];

MyMath::MyMath()
{
}


MyMath::~MyMath()
{
}

void MyVector2::DirUpdate() {
	DeltaDirArr[L] = DLeft = LEFT * MyTime::DeltaTime();
	DeltaDirArr[R] = DRight = RIGHT * MyTime::DeltaTime();
	DeltaDirArr[T] = DTop = UP * MyTime::DeltaTime();
	DeltaDirArr[B] = DBottom = DOWN * MyTime::DeltaTime();
}

const MyVector2 & MyVector2::DeltaDir(MONSTER_DIR dir)
{
	return DeltaDirArr[dir];
}

