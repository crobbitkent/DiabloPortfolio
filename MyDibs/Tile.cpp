#include "Tile.h"
#include "MapGenerator.h"
#include "Map.h"

Tile::Tile()
{
	mNeighbors.reserve(8);
}

Tile::Tile(MyVector2 pos, wchar_t basePixel, Map* map)
	: mMap(map)
{
	mNeighbors.reserve(8);
}

Tile::~Tile()
{

}

void Tile::Init(MyVector2 pos, wchar_t basePixel, Map * map)
{
	Pos(pos);
	mMap = map;
	// Pixel(basePixel);
}

void Tile::SetWall(wchar_t pixel)
{
	mIsWalkable = false;
	// Pixel(pixel);

	mPrevPixel = pixel;

	/*switch (pixel)
	{
	case PIXEL_N:
		mTileType = TT_TOPWALL;
		break;
	case PIXEL_NW:
		mTileType = TT_LEFTTOPWALL;
		break;
	case PIXEL_W:
		mTileType = TT_LEFTWALL;
		break;
	case PIXEL_WS:
		mTileType = TT_LEFTDOWNWALL;
		break;
	case PIXEL_S:
		mTileType = TT_BOTTOMWALL;
		break;
	case PIXEL_SE:
		mTileType = TT_RIGHTDOWNWALL;
		break;
	case PIXEL_E:
		mTileType = TT_RIGHTWALL;
		break;
	case PIXEL_EN:
		mTileType = TT_RIGHTTOPWALL;
		break;
	default:
		mTileType = TT_WALL;
		break;
	}*/
}

void Tile::SetWall()
{
	mIsWalkable = false;
	// Pixel(PIXEL_WALL);
	mTileType = TT_WALL;

	mPrevPixel = PIXEL_WALL;
}

void Tile::SetFloor()
{
	mIsWalkable = true;
	// Pixel(PIXEL_FLOOR);
	mTileType = TT_FLOOR;

	mPrevPixel = PIXEL_FLOOR;
}

void Tile::SetFloor(wchar_t special)
{
	mIsWalkable = true;
	// Pixel(special);
	mTileType = TT_FLOOR;

	mPrevPixel = special;
}

void Tile::SetLeftDoor()
{
	// Pixel(PIXEL_LEFT_DOOR);
	mTileType = TT_LEFT_DOOR;

	mIsWalkable = true;

	mPrevPixel = PIXEL_LEFT_DOOR;
}

void Tile::SetTopDoor()
{
	// Pixel(PIXEL_TOP_DOOR);
	mTileType = TT_TOP_DOOR;

	mIsWalkable = true;

	mPrevPixel = PIXEL_TOP_DOOR;
}

void Tile::SetRightDoor()
{
	// Pixel(PIXEL_RIGHT_DOOR);
	mTileType = TT_RIGHT_DOOR;

	mIsWalkable = true;

	mPrevPixel = PIXEL_RIGHT_DOOR;
}

void Tile::SetBottomDoor()
{
	// Pixel(PIXEL_BOTTOM_DOOR);
	mTileType = TT_BOTTOM_DOOR;

	mIsWalkable = true;

	mPrevPixel = PIXEL_BOTTOM_DOOR;
}

void Tile::SetUpstair()
{
	// Pixel(PIXEL_UPSTAIR);
	mTileType = TT_UPSTAIR;

	mIsUpstair = true;

	mPrevPixel = PIXEL_UPSTAIR;
}

void Tile::SetDownstair()
{
	// Pixel(PIXEL_DOWNSTAIR);
	mTileType = TT_DOWNSTAIR;

	mIsDownstair = true;

	mPrevPixel = PIXEL_DOWNSTAIR;
}

void Tile::SetCorner(TILE_TYPE cornerType)
{
	mTileType = cornerType;

	//switch (cornerType)
	//{
	//case TT_LEFTDOWNWALL:
	//	Pixel(PIXEL_WS);
	//	break;
	//case TT_RIGHTDOWNWALL:
	//	Pixel(PIXEL_SE);
	//	break;
	//case TT_RIGHTTOPWALL:
	//	Pixel(PIXEL_EN);
	//	break;
	//case TT_LEFTTOPWALL:
	//	Pixel(PIXEL_NW);
	//	break;
	//default:
	//	break;
	//}

}

void Tile::SetUnit(MyActor* obj)
{
	mUnit = obj;

	// Pixel(obj->Pixel());
}

void Tile::SetItem(MyActor* obj)
{
	mItem = obj;

	// Pixel(obj->Pixel());
}

void Tile::UnsetItem()
{
}

void Tile::UnsetUnit()
{
	mUnit = nullptr;

	// Pixel(mPrevPixel);
}




bool Tile::IsWall(MyVector2 _Pos)
{
	// return GameManager::GetMap()->GetTile(_Pos.X, _Pos.Y).IsWall();

	return true;
}

// true¸é À¯´ÖÀÌ ÀÖÀ½
bool Tile::IsUnitOn(MyVector2 _Pos)
{
	// return GameManager::GetMap()->GetTile(_Pos.X, _Pos.Y).IsUnitOn();

	return true;
}

// true¸é À¯´ÖÀÌ ÀÖÀ½
bool Tile::IsUnitOn()
{
	if (nullptr == mUnit)
	{
		return false;
	}

	return true;
}

void Tile::FindNeighbor()
{
	// ¿ì
	if (false == OverCheck({ Pos().X + 1, Pos().Y }))
	{
		mNeighbors.push_back(&mMap->GetTile(Pos().X + 1, Pos().Y));
	}

	// ¿ì»ó
	if (false == OverCheck({ Pos().X + 1, Pos().Y + 1 }))
	{
		mNeighbors.push_back(&mMap->GetTile(Pos().X + 1, Pos().Y + 1));
	}
	// »ó
	if (false == OverCheck({ Pos().X, Pos().Y + 1 }))
	{
		mNeighbors.push_back(&mMap->GetTile(Pos().X, Pos().Y + 1));
	}
	// ÁÂ»ó
	if (false == OverCheck({ Pos().X - 1, Pos().Y + 1 }))
	{
		mNeighbors.push_back(&mMap->GetTile(Pos().X - 1, Pos().Y + 1));
	}
	// ÁÂ
	if (false == OverCheck({ Pos().X - 1, Pos().Y }))
	{
		mNeighbors.push_back(&mMap->GetTile(Pos().X - 1, Pos().Y));
	}
	// ÁÂÇÏ
	if (false == OverCheck({ Pos().X - 1, Pos().Y - 1 }))
	{
		mNeighbors.push_back(&mMap->GetTile(Pos().X - 1, Pos().Y - 1));
	}
	// ÇÏ
	if (false == OverCheck({ Pos().X , Pos().Y - 1 }))
	{
		mNeighbors.push_back(&mMap->GetTile(Pos().X, Pos().Y - 1));
	}
	// ¿ìÇÏ
	if (false == OverCheck({ Pos().X + 1, Pos().Y - 1 }))
	{
		mNeighbors.push_back(&mMap->GetTile(Pos().X + 1, Pos().Y - 1));
	}
}

void Tile::ClearTile()
{
	mG = 0;
	mF = 0;
	mH = 0;

	mParentTile = nullptr;
	mUnit = nullptr;
}

float Tile::GetTileCost() const
{
	if (true == mIsWalkable)
	{
		return 1;
	}

	return 99;
}



// (float)startX * (TILE_WIDTH + 1), (float)startY * (TILE_HEIGHT + 3)

