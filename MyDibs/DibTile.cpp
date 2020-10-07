#include "DibTile.h"

#include <MyInput.h>
#include <MyTextureManager.h>
#include <MyMacro.h>
#include <MyImage.h>

#include "RandomMap.h"
#include "Player.h"

int DibTile::mUpstairCount = 0;
int DibTile::mDownstairCount = 0;

DibTile::DibTile()
{
}

DibTile::DibTile(const DibTile & other)
	: mMap(other.mMap)
	, mX(other.mX)
	, mY(other.mY)
	, mIsWall(other.mIsWall)
	, mRoom(other.mRoom)
	, mIsUpstair(other.mIsUpstair)
	, mIsDownstair(other.mIsDownstair)
	// , mCornerType(other.mCornerType)
	, mWallType(other.mWallType)
	, mTileType(other.mTileType)
	, mPlayer(other.mPlayer)
	, mIsInRoom(false)
{
	int a = 0;


}

DibTile::DibTile(RandomMap* map, int x, int y, LEAGUE_ORDER order)
	: mMap(map)
	, mX(x)
	, mY(y)
	, mRenderOrder(order)
	, mIsWall(true)
	, mRoom(nullptr)
	, mIsUpstair(false)
	, mIsDownstair(false)
	// , mCornerType(CT_NONE)
	, mWallType(WT_NONE)
	, mTileType(TT_INWALL)
	, mIsDebug(true)
{
	mMoveSpeed = 800.0f;

	mPlayer = mMap->mPlayer;

	/*mTileRenderer = CreateRenderer<MyRenderer>(LO_TILE);

	mTileRenderer->SubSize({ TILE_WIDTH, TILE_HEIGHT + 2 });
	mTileRenderer->Image(L"ChurchDungeonTile.bmp");
	mTileRenderer->TransColor(255, 255, 255);
	mTileRenderer->SubPos({ 44, 8 - 64 });
	mTileRenderer->On();*/
	// --------------------------------------------------------------------------------

	mWallRenderer = CreateRenderer<MyRenderer>(order);

	mWallRenderer->SubSize({ TILE_WIDTH, TILE_HEIGHT + 2 });
	mWallRenderer->Image(L"ChurchDungeonWall.bmp");
	mWallRenderer->TransColor(255, 255, 255);
	mWallRenderer->SubPos({ 44 + 3, -64 });
	mWallRenderer->SetRenderStyle(RS_ALPHA);
	mWallRenderer->Off();
	// --------------------------------------------------------------------------------

	mFogRenderer = CreateRenderer<MyRenderer>(order);

	mFogRenderer->SubSize({ TILE_WIDTH, TILE_HEIGHT + 2 });
	mFogRenderer->Image(L"ChurchDungeonWallBlack.bmp");
	mFogRenderer->TransColor(255, 255, 255);
	mFogRenderer->SubPos({ 44 + 3, -64 });

	mFogRenderer->SetRenderStyle(RS_ALPHA);
	mFogRenderer->Off();


	// 타일 위치 설정
	float offsetX = (TILE_OFFSETX) / 2.f;
	float offsetY = offsetX / 2.f;

	float startX = MAPSIZE_X / 2.f - offsetX - (y * offsetX);
	float startY = y * offsetY;

	float newX = (startX + x * offsetX);
	float newY = startY + (x * offsetY);

	Pos(MyVector2(newX /*+ 44*/, newY /*+ 8*/));

	DebugSetTypeName();


	/////////////////////////////////////////////////
}

DibTile::~DibTile()
{
}

//void DibTile::Init()
//{
//	mAllBigFog.reserve(MAPCOUNT_X * MAPCOUNT_Y);
//	mAllWall.reserve(MAPCOUNT_X * MAPCOUNT_Y);
//	mAllTile.reserve(MAPCOUNT_X * MAPCOUNT_Y);
//
//
//}

void DibTile::SetFloor()
{
	mIsWall = false;
	// Pixel(PIXEL_FLOOR);
	mTileType = TT_FLOOR;
	mWallType = WT_NONE;
}

void DibTile::SetUpstair()
{
	if (0 == mUpstairCount || 1 == mUpstairCount || 2 == mUpstairCount)
	{
		if (WT_DOWNWALL == mWallType)
		{
			return;
		}
	}

	++mUpstairCount;

	mTileType = TT_STAIR;
	mIsUpstair = true;

	switch (mUpstairCount)
	{
	case 1:
		mWallType = WT_UP1;
		break;
	case 2:
		mWallType = WT_UP2;
		break;
	case 3:
		mWallType = WT_UP3;
		break;
	case 4:
		mWallType = WT_UP4;
		break;
	case 5:
		mWallType = WT_UP5;
		break;
	case 6:
		mWallType = WT_UP6;
		break;
	case 7:
		mWallType = WT_UP7;
		break;
	case 8:
		mWallType = WT_UP8;
		break;
	case 9:
		mWallType = WT_UP9;
		mUpstairCount = 0;
		break;

	}
}

void DibTile::SetDownstair()
{
	++mDownstairCount;

	mTileType = TT_STAIR;
	mIsDownstair = true;

	switch (mDownstairCount)
	{
	case 1:
		mWallType = WT_DOWN1;
		break;
	case 2:
		mWallType = WT_DOWN2;
		break;
	case 3:
		mWallType = WT_DOWN3;
		break;
	case 4:
		mWallType = WT_DOWN4;
		break;
	case 5:
		mWallType = WT_DOWN5;
		break;
	case 6:
		mWallType = WT_DOWN6;
		mDownstairCount = 0;
		break;
	}
}

bool DibTile::IsRoom()
{
	if (nullptr == mRoom)
	{
		return false;
	}

	return true;
}

void DibTile::DebugSwitch()
{
	if (true == mIsDebug)
	{
		mIsDebug = false;
	}
	else
	{
		mIsDebug = true;
	}
}

void DibTile::DebugRender()
{
	wchar_t Arr[256] = { 0, };
	swprintf_s(Arr, L"%d, %d", mX, mY);
	TextOutW(MyTextureManager::BackDC(), CalPos().IntX() + 44, CalPos().IntY() + 8 - 64, Arr, lstrlen(Arr));
}

void DibTile::DebugOutputTile()
{
	wchar_t Arr[200] = { 0, };
	swprintf_s(Arr, L"Tile = %d, %d  ", mX, mY);
	OutputDebugStringW(Arr);
	swprintf_s(Arr, L"TileType = ");
	OutputDebugStringW(Arr);
	swprintf_s(Arr, mAllTileType[(int)mWallType]);
	OutputDebugStringW(Arr);
	swprintf_s(Arr, L"\n");
	OutputDebugStringW(Arr);
}

void DibTile::DebugSetTypeName()
{
	mAllTileType[WT_NONE] = L"WT_NONE";
	mAllTileType[WT_FLOOR] = L"WT_FLOOR";

	mAllTileType[WT_LEFTWALL] = L" WT_LEFTWALL";
	mAllTileType[WT_RIGHTWALL] = L" WT_RIGHTWALL";
	mAllTileType[WT_TOPWALL] = L" WT_TOPWALL";
	mAllTileType[WT_DOWNWALL] = L" WT_DOWNWALL";

	mAllTileType[WT_LASTLEFT1] = L"WT_LASTLEFT1";
	mAllTileType[WT_LASTLEFT2] = L"WT_LASTLEFT2";
	mAllTileType[WT_LASTRIGHT1] = L"WT_LASTRIGHT1";
	mAllTileType[WT_LASTRIGHT2] = L"WT_LASTRIGHT2";

	mAllTileType[WT_LEFTDOWN] = L" WT_LEFTDOWN";
	mAllTileType[WT_RIGHTDOWN] = L"	WT_RIGHTDOWN";
	mAllTileType[WT_RIGHTTOP] = L" WT_RIGHTTOP";
	mAllTileType[WT_LEFTTOP] = L" WT_LEFTTOP";
	mAllTileType[WT_LEFTDOWNWALL] = L"WT_LEFTDOWNWALL";
	mAllTileType[WT_RIGHTDOWNWALL] = L"WT_RIGHTDOWNWALL";
	mAllTileType[WT_RIGHTTOPWALL] = L"WT_RIGHTTOPWALL";
	mAllTileType[WT_LEFTTOPWALL] = L"WT_LEFTTOPWALL";
	mAllTileType[WT_COLUMN] = L"WT_COLUMN";
	mAllTileType[WT_COLUMNSHADOW1] = L" WT_COLUMNSHADOW1";
	mAllTileType[WT_COLUMNSHADOW2] = L"WT_COLUMNSHADOW2";



	mAllTileType[WT_UP1] = L"WT_UP1";
	mAllTileType[WT_UP2] = L"WT_UP2";
	mAllTileType[WT_UP3] = L"WT_UP3";
	mAllTileType[WT_UP4] = L"WT_UP4";
	mAllTileType[WT_UP5] = L"WT_UP5";
	mAllTileType[WT_UP6] = L"WT_UP6";
	mAllTileType[WT_UP7] = L"WT_UP7";
	mAllTileType[WT_UP8] = L"WT_UP8";
	mAllTileType[WT_UP9] = L"WT_UP9";
	mAllTileType[WT_DOWN1] = L"WT_DOWN1";
	mAllTileType[WT_DOWN2] = L"WT_DOWN2";
	mAllTileType[WT_DOWN3] = L"WT_DOWN3";
	mAllTileType[WT_DOWN4] = L"WT_DOWN4";

	mAllTileType[WT_DOWN5] = L"WT_DOWN5";
	mAllTileType[WT_DOWN6] = L"WT_DOWN6";

	mAllTileType[WT_LEFTDOOR] = L"WT_LEFTDOOR";
	mAllTileType[WT_TOPDOOR] = L"WT_TOPDOOR";
	mAllTileType[WT_RIGHTDOOR] = L" WT_RIGHTDOOR";
	mAllTileType[WT_BOTTOMDOOR] = L"WT_BOTTOMDOOR";

	// 그림자
	mAllTileType[WT_LDSHADOW1] = L"WT_LDSHADOW1";
	mAllTileType[WT_LDSHADOW2] = L"WT_LDSHADOW2";
	mAllTileType[WT_TOPSHADOW] = L"WT_TOPSHADOW";
	mAllTileType[WT_RIGHTTOPSHADOW] = L"WT_RIGHTTOPSHADOW";

	mAllTileType[WT_CANDLESHADOW1] = L"WT_CANDLESHADOW1";
	mAllTileType[WT_CANDLESHADOW2] = L"WT_CANDLESHADOW2";

	mAllTileType[WT_BOXSHADOW] = L"WT_BOXSHADOW";

	// 아치
	mAllTileType[WT_ARCH_LEFT] = L"WT_ARCH_LEFT";
	mAllTileType[WT_ARCH_TOP] = L"WT_ARCH_TOP";


	mAllTileType[WT_ARCH_LEFTWALL] = L"WT_ARCH_LEFTWALL";
	mAllTileType[WT_ARCH_LASTLEFT] = L"WT_ARCH_LASTLEFT";
	mAllTileType[WT_ARCH_TOPWALL] = L"WT_ARCH_TOPWALL";
	mAllTileType[WT_ARCH_LASTTOP] = L"WT_ARCH_LASTTOP";
	mAllTileType[WT_ARCH_LEFTTOP] = L"WT_ARCH_LEFTTOP";

	mAllTileType[WT_ARCH_TOPSHADOW] = L"WT_ARCH_TOPSHADOW";
	mAllTileType[WT_ARCH_TOPSHADOWWALL] = L"WT_ARCH_TOPSHADOWWALL";


	// 바닥 스타일
	mAllTileType[WT_CANDLE] = L"WT_CANDLE";
	mAllTileType[WT_BIGBOX] = L"WT_BIGBOX";
	mAllTileType[WT_STONE1] = L"WT_STONE1";
	mAllTileType[WT_STONE2] = L"WT_STONE2";
	mAllTileType[WT_STONE3] = L"WT_STONE3";
	mAllTileType[WT_STONE4] = L"WT_STONE4";
	mAllTileType[WT_STONE5] = L"WT_STONE5";
	mAllTileType[WT_STONE6] = L"WT_STONE6";
	mAllTileType[WT_BLOOD1] = L"WT_BLOOD1";
	mAllTileType[WT_BLOOD2] = L"WT_BLOOD2";
	mAllTileType[WT_BLOOD3] = L"WT_BLOOD3";

	// 벽 스타일
	mAllTileType[WT_BLOOD_TOPWALL1] = L"WT_BLOOD_TOPWALL1";
	mAllTileType[WT_BLOOD_TOPWALL2] = L"WT_BLOOD_TOPWALL2";
	mAllTileType[WT_BLOOD_TOPWALL3] = L"WT_BLOOD_TOPWALL3";
	mAllTileType[WT_BLOOD_LEFTWALL1] = L"WT_BLOOD_LEFTWALL1";
	mAllTileType[WT_BLOOD_LEFTWALL2] = L"WT_BLOOD_LEFTWALL2";
	mAllTileType[WT_STATUE_LEFTWALL1] = L"WT_STATUE_LEFTWALL1";
	mAllTileType[WT_STATUE_LEFTWALL2] = L"WT_STATUE_LEFTWALL2";
	mAllTileType[WT_STATUE_LEFTWALL3] = L"WT_STATUE_LEFTWALL3";
	mAllTileType[WT_STATUE_LEFTWALL4] = L"WT_STATUE_LEFTWALL4";
	mAllTileType[WT_STATUE_TOPWALL1] = L"WT_STATUE_TOPWALL1";
	mAllTileType[WT_STATUE_TOPWALL2] = L"WT_STATUE_TOPWALL2";
	mAllTileType[WT_STATUE_TOPWALL3] = L"WT_STATUE_TOPWALL3";
	mAllTileType[WT_STATUE_TOPWALL4] = L"WT_STATUE_TOPWALL4";

	mAllTileType[WT_BROKEN_TOPWALL1] = L"WT_BROKEN_TOPWALL1";
	mAllTileType[WT_BROKEN_TOPWALL2] = L"WT_BROKEN_TOPWALL2";
	mAllTileType[WT_BROKEN_TOPWALL3] = L"WT_BROKEN_TOPWALL3";
	mAllTileType[WT_BROKEN_LEFTWALL1] = L"WT_BROKEN_LEFTWALL1";
	mAllTileType[WT_BROKEN_LEFTWALL2] = L"WT_BROKEN_LEFTWALL2";

	// 쇠창살
	mAllTileType[WT_IRON_TOPWALL] = L"WT_IRON_TOPWALL";
	mAllTileType[WT_IRON_LEFTWALL] = L"WT_IRON_LEFTWALL";
	mAllTileType[WT_IRON_DOWNWALL] = L"WT_IRON_DOWNWALL";
	mAllTileType[WT_IRON_RIGHTWALL] = L"WT_IRON_RIGHTWALL";

	mAllTileType[WT_IRON_LEFTTOP_LEFT] = L"WT_IRON_LEFTTOP_LEFT";
	mAllTileType[WT_IRON_LEFTTOP_RIGHT] = L"WT_IRON_LEFTTOP_RIGHT";

	mAllTileType[WT_IRON_LEFTDOWN] = L"WT_IRON_LEFTDOWN";
	mAllTileType[WT_IRON_RIGHTDOWN] = L"WT_IRON_RIGHTDOWN";
	/*mAllTileType[WT_IRON_RIGHTTOP]= L"WT_IRON_RIGHTTOP ";
	mAllTileType[WT_IRON_LEFTTOP] = L"WT_IRON_LEFTTOP";*/

	mAllTileType[WT_ARCH_LASTTOP_SHADOW] = L"WT_ARCH_LASTTOP_SHADOW";
	mAllTileType[WT_ARCH_LDSHADOW3] = L"WT_ARCH_LDSHADOW3";
	mAllTileType[WT_ARCH_LEFTWALL_SHADOW] = L"WT_ARCH_LEFTWALL_SHADOW";
	mAllTileType[WT_ARCH_RIGHTDOWNWALL_SHADOW] = L"WT_ARCH_RIGHTDOWNWALL_SHADOW";
	mAllTileType[WT_ARCH_RIGHTTOPWALL] = L"WT_ARCH_RIGHTTOPWALL";

	mAllTileType[WT_ARCH_TOPWALL_SHADOW] = L"WT_ARCH_TOPWALL_SHADOW";

	mAllTileType[WT_LEFTWALL1_B] = L"WT_LEFTWALL1_B";
	mAllTileType[WT_LEFTWALL2_B] = L"WT_LEFTWALL2_B";
	mAllTileType[WT_TOPWALL1_B] = L"WT_TOPWALL1_B";
	mAllTileType[WT_TOPWALL2_B] = L"WT_TOPWALL2_B";
	mAllTileType[WT_LEFTTOPWALL_B] = L"WT_LEFTTOPWALL_B";
	mAllTileType[WT_FLOOR1_B] = L"WT_FLOOR1_B";
	mAllTileType[WT_FLOOR2_B] = L"WT_FLOOR2_B";
	mAllTileType[WT_FLOOR3_B] = L"WT_FLOOR3_B";
	mAllTileType[WT_FLOOR4_B] = L"WT_FLOOR4_B";

	mAllTileType[WT_BDOOR] = L"WT_BDOOR";
}

void DibTile::Update()
{
	if (2 == mLevel)
	{
		int a = 0;
	}

	if (true == MyInput::Down(L"OUTPUT"))
	{
		DebugOutputTile();
	}

	if (nullptr == mPlayer->GetDibTile())
	{
		return;
	}

	TileIndex tileIndex = { mX, mY };

	TileIndex playerTileIndex = mPlayer->DibTileIndex();


	TileIndex tileDis = tileIndex - playerTileIndex;
	TileIndex lastDis;

	lastDis.X = abs(tileDis.X);
	lastDis.Y = abs(tileDis.Y);

	int gap = lastDis.X + lastDis.Y;

	gap = abs(gap);

	if (10 < gap)
	{
		mFogRenderer->Off();
		mWallRenderer->Off();
		return;
	}

	if (LO_UPTILE == mRenderOrder)
	{
		if (mPlayer->GetDibTile()->mX < mX || mPlayer->GetDibTile()->mY < mY)
		{
			// mTileRenderer->Off();
			mWallRenderer->Off();
			mFogRenderer->Off();
		}
		else
		{
			// mTileRenderer->On();
			mWallRenderer->On();
			mFogRenderer->On();
		}
	}

	if (LO_DOWNTILE == mRenderOrder)
	{
		if (mPlayer->GetDibTile()->mX >= mX && mPlayer->GetDibTile()->mY >= mY)
		{
			// mTileRenderer->Off();
			mWallRenderer->Off();
			mFogRenderer->Off();
		}
		else
		{
			// mTileRenderer->On();
			mWallRenderer->On();
			mFogRenderer->On();
		}
	}


	mIncrease = 30;
	mBase = 15;


	if (1 >= gap)
	{
		mFogRenderer->SetBF(0);
	}
	else if (2 >= gap)
	{
		mFogRenderer->SetBF(mBase + mIncrease * 1);
	}
	else if (3 >= gap)
	{
		mFogRenderer->SetBF(mBase + mIncrease * 3);
	}
	else if (4 >= gap)
	{
		mFogRenderer->SetBF(mBase + mIncrease * 4);
	}
	else if (5 >= gap)
	{
		mFogRenderer->SetBF(mBase + mIncrease * 5);
	}
	else if (6 >= gap)
	{
		mFogRenderer->SetBF((int)(mBase + mIncrease * 6.5));
	}
	else if (7 >= gap)
	{
		mFogRenderer->SetBF(mBase + mIncrease * 7);
	}
	else
	{
		mFogRenderer->SetBF(mBase + mIncrease * 7);
	}


}

void DibTile::LastCheck1()
{
	if (TT_FLOOR == mMap->GetTile({ mX, mY - 1 })->TileType())
	{
		mWallType = WT_DOWNWALL;
	}
}

void DibTile::LastCheck2()
{
	if (TT_FLOOR == mMap->GetTile({ mX, mY - 1 })->TileType())
	{
		mWallType = WT_LEFTTOPWALL;
	}
}

