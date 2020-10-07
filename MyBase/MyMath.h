#pragma once

#include "MyTime.h"

#include "MyMacro.h"

#include <math.h>

class MyVector2
{
public:
	static void DirUpdate();

	MyVector2() : X(0.0f), Y(0.0f)	{}
	MyVector2(float x, float y) : X(x), Y(y) {}
	MyVector2(int x, int y) : X((float)x), Y((float)y) {}

	float HalfX() const { return X * 0.5f; }
	float HalfY() const { return Y * 0.5f; }

	static const MyVector2& DeltaLeft() { return DLeft; }
	static const MyVector2& DeltaRight() { return DRight; }
	static const MyVector2& DeltaTop() { return DTop; }
	static const MyVector2& DeltaBottom() { return DBottom; }
	static const MyVector2& DeltaDir(MONSTER_DIR dir);
public:
	static const MyVector2 ZERO;

	static const MyVector2 LEFT;
	static const MyVector2 LEFTDOWN;
	static const MyVector2 DOWN;
	static const MyVector2 DOWNRIGHT;
	static const MyVector2 RIGHT;
	static const MyVector2 RIGHTUP;
	static const MyVector2 UP;
	static const MyVector2 UPLEFT;

	static const MyVector2 ARRDIR[8];

	float X;
	float Y;

private:
	static MyVector2 DLeft;
	static MyVector2 DRight;
	static MyVector2 DTop;
	static MyVector2 DBottom;

	static MyVector2 DeltaDirArr[MD_MAX];

///////////////////////////// operator overloading /////////////////////////	
public:
	bool operator==(const MyVector2& _Pos) const
	{
		return X == _Pos.X && Y == _Pos.Y;
	}
	
	bool operator!=(const MyVector2& _Pos) const
	{
		return X != _Pos.X || Y != _Pos.Y;
	}

	bool operator>=(const MyVector2& _Pos) const
	{
		return X >= _Pos.X && Y >= _Pos.Y;
	}

	bool operator<=(const MyVector2& _Pos) const
	{
		return X <= _Pos.X && Y <= _Pos.Y;
	}

	MyVector2 operator *(const float _Value) const
	{
		return { this->X * _Value, this->Y * _Value };
	}

	MyVector2 operator /(const float _Value) const
	{
		return { this->X / _Value, this->Y / _Value };
	}

	MyVector2& operator *=(const float _Value)
	{
		this->X *= _Value;
		this->Y *= _Value;
		return *this;
	}

	MyVector2 operator +(const MyVector2& other) const
	{
		return { this->X + other.X, this->Y + other.Y };
	}

	MyVector2 operator +(const int other) const
	{
		return { this->X + other, this->Y + other };
	}


	MyVector2& operator +=(const MyVector2& other)
	{
		this->X += other.X;
		this->Y += other.Y;
		return *this;
	}



	MyVector2 operator -(const MyVector2& other) const
	{
		return { this->X - other.X, this->Y - other.Y };
	}

	MyVector2& operator -=(const MyVector2& other)
	{
		this->X -= other.X;
		this->Y -= other.Y;
		return *this;
	}



	/////////////////////// normal Fuction /////////////////
	inline int IntX() const {		return (int)X;	}
	inline int IntY() const {		return (int)Y;	}
	inline int IntHalfX() const {		return (int)HalfX();	}
	inline int IntHalfY() const {		return (int)HalfY();	}
	inline float Length() const { return sqrtf((X*X) + (Y*Y)); }
};

class MyRect
{
public:
	float Left()	const  { return Pos.X - Size.HalfX(); }
	float Right()	const { return Pos.X + Size.HalfX(); }
	float Top()		const { return Pos.Y - Size.HalfY(); }
	float Bottom()	const  { return Pos.Y + Size.HalfY(); }

	int IntLeft()	const { return (int)Left(); }
	int IntRight()	const { return (int)Right(); }
	int IntTop()	const { return (int)Top(); }
	int IntBottom() const { return (int)Bottom(); }

public:
	MyVector2 Pos;
	MyVector2 Size;
};


class MyMath
{
public:
	// v0 = start vector2, v1 = dest vector2, t = deltaTime
	static MyVector2 Lerp(MyVector2 v0, MyVector2 v1, float t) {
		if (1.f <= t)
		{
			t = 1.f;
		}

		return (v1 - v0) * t;
	}

	/*float lerp(float p1, float p2, float d1) {
		return (1 - d1)*p1 + d1 * p2;
	}*/

	//static MyVector2 Lerp(MyVector2 start, MyVector2 dest, float time) {
	//	float tempTime = time;

	//	if (1.f <= tempTime)
	//	{
	//		tempTime = 1.f;
	//	}

	//	return  start * (1 - tempTime) + dest * tempTime;
	//	// return v0 + (v1 - v0) * t;
	//}

	static bool RectToRect(MyVector2 _LeftPos, MyVector2 _LeftSize, MyVector2 _RightPos, MyVector2 _RightSize)
	{
		return RectToRect({ _LeftPos , _LeftSize }, { _RightPos , _RightSize });
	}
	static bool RectToCircle(MyVector2 _LeftPos, MyVector2 _LeftSize, MyVector2 _RightPos, float _RightSize)
	{
		return false;
	}
	static bool RectToPoint(MyVector2 _LeftPos, MyVector2 _LeftSize, MyVector2 _RightPos)
	{
		return false;
	}

	static bool CircleToCircle(MyVector2 _LeftPos, float _LeftSize, MyVector2 _RightPos, float _RightSize)
	{
		return false;
	}
	static bool CircleToRect(MyVector2 _LeftPos, float _LeftSize, MyVector2 _RightPos, MyVector2 _RightSize)
	{
		return false;
	}
	static bool CircleToPoint(MyVector2 _LeftPos, float _LeftSize, MyVector2 _RightPos)
	{
		return false;
	}

	static bool PointToPoint(MyVector2 _LeftPos, MyVector2 _RightPos) {
		return false;
	}
	static bool PointToRect(MyVector2 _LeftPos, MyVector2 _RightPos, MyVector2 _RightSize) {
		return false;
	}
	static bool PointToCircle(MyVector2 _LeftPos, MyVector2 _RightPos, float _RightSize) {
		return false;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static bool __stdcall RectToRect(const MyRect& _Left, const MyRect& _Right)
	{
		if (_Left.Right() < _Right.Left())
		{
			return false;
		}

		if (_Left.Left() > _Right.Right())
		{
			return false;

		}

		if (_Left.Top() > _Right.Bottom())
		{
			return false;
		}

		if (_Left.Bottom() < _Right.Top())
		{
			return false;
		}

		return true;
	}

	static bool __stdcall RectToCircle(const MyRect& _Left, const MyRect& _Right)
	{
		return false;
	}
	static bool __stdcall RectToPoint(const MyRect& _Left, const MyRect& _Right)
	{
		return false;
	}

	static bool __stdcall CircleToCircle(const MyRect& _Left, const MyRect& _Right)
	{
		return false;
	}
	static bool __stdcall CircleToRect(const MyRect& _Left, const MyRect& _Right)
	{
		return false;
	}
	static bool __stdcall CircleToPoint(const MyRect& _Left, const MyRect& _Right)
	{
		return false;
	}

	static bool __stdcall PointToPoint(const MyRect& _Left, const MyRect& _Right)
	{
		return false;
	}
	static bool __stdcall PointToRect(const MyRect& _Left, const MyRect& _Right)
	{
		return false;
	}
	static bool __stdcall PointToCircle(const MyRect& _Left, const MyRect& _Right)
	{
		return false;
	}





public:
	MyMath();
	~MyMath();
};

