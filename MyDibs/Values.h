#pragma once


#include <PathManager.h>

#include <iostream>
#include <assert.h>
#include <Windows.h>

#include <MyMath.h>
#include <MyMacro.h>

#define BACK_TILE_X 5120
#define BACK_TILE_Y 2686

static const MyVector2 PlayerHorizontal = { 128.f / 2, 0.f };
static const MyVector2 PlayerVertical = { 0.f , 64.f / 2 };
static const MyVector2 PlayerDiagonal = { 128.f / 4 , 64.f / 4 };


static int seed = 100;

// true면 넘었다.
bool OverCheck(MyVector2 pos);
// int RandomNumber(int min, int max);



enum RENDER_ORDER
{
	RO_FIRST,
	RO_SECOND,
	RO_MAX
};

enum CHEST_TYPE
{
	/*CHT_BARREL,
	CHT_SARCO,
	CHT_BODY,
	CHT_W_RACK,
	CHT_A_RACK,
	CHT_BOOKCASE,
	CHT_POST,
	CHT_MAX*/
};

class ItemData
{
public:
	std::wstring mName;
	int mDamage = 0;
	int mMaxDamage = 0;
	int mArmor = 0;
	int mMaxArmor = 0;
	int mDurability = 0;
	CHAR_STATS mRequire1 = CS_MAX;
	CHAR_STATS mRequire2 = CS_MAX;
	int mRequire1Amount = 0;
	int mRequire2Amount = 0;
	int mPrice = 0;
	int mItemLevel = 0;
	ITEM_TYPE mType = IT_MAX;

	int mStr = 0;
	int mDex = 0;
	int mVit = 0;
	int mMag = 0;

	int X = 0;
	int Y = 0;

	int gold = 0;
	// Gold: 30.3%
	// Item: 10.7%
	// Nothing : 59.0%
	float mDropRate = 0.f;
};

class UnitData
{
public:
	const wchar_t* name = {};
	int dlevel = 1;
	int mLevel = 1;
	int mMaxHP = 0;
	int mMaxMP = 0;
	int armor = 0;
	int toHit = 0;
	int attackMin = 0;
	int attackMax = 0;

	int gold = 0;
	int exp = 0;

};





//class Position
//{
//public:
//	bool operator==(const Position& _Pos) const
//	{
//		return X == _Pos.X && Y == _Pos.Y;
//	}
//
//	bool operator!=(const Position& _Pos) const
//	{
//		return X != _Pos.X || Y != _Pos.Y;
//	}
//
//	Position& operator=(const Position& _Pos)
//	{
//		X = _Pos.X;
//		Y = _Pos.Y;
//		return *this;
//	}
//
//	Position operator*(const int _Value) const
//	{
//		Position Return = Position();
//		Return.X = X * _Value;
//		Return.Y = Y * _Value;
//		return Return;
//	}
//
//	Position& operator*=(const int _Value)
//	{
//		X *= _Value;
//		Y *= _Value;
//		return *this;
//	}
//
//	Position operator+(const Position& _Pos) const
//	{
//		Position Return = Position();
//		Return.X = X + _Pos.X;
//		Return.Y = Y + _Pos.Y;
//		return Return;
//	}
//
//	Position operator-(const Position& _Pos) const
//	{
//		Position Return = Position();
//		Return.X = X - _Pos.X;
//		Return.Y = Y - _Pos.Y;
//		return Return;
//	}
//
//	Position& operator+=(const Position& _Pos)
//	{
//		X += _Pos.X;
//		Y += _Pos.Y;
//		return *this;
//	}
//
//	Position() : X(0), Y(0)
//	{
//	}
//
//	Position(const Position& _Pos) : X(_Pos.X), Y(_Pos.Y)
//	{
//	}
//
//	Position(int _X, int _Y) : X(_X), Y(_Y)
//	{
//	}
//
//public:
//	static const Position LEFT;
//	static const Position LEFTDOWN;
//	static const Position DOWN;
//	static const Position DOWNRIGHT;
//	static const Position RIGHT;
//	static const Position RIGHTUP;
//	static const Position UP;
//	static const Position UPLEFT;
//	static const Position ARRDIR[8];
//
//	int X;
//	int Y;
//};
