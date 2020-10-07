#include "TileColRenderer.h"

#include <MyTextureManager.h>
#include <MyImage.h>
#include <MyActor.h>
#include <MyInput.h>

#include "Values.h"

#include "Monster.h"
#include "Player.h"



bool TileColRenderer::mIsDebugging = false;

TileColRenderer::TileColRenderer()
	: mIsFog(false)
{
	MyRenderer::TransColor(255, 255, 255);

	// SetBF(100);
}


TileColRenderer::~TileColRenderer()
{
}

void TileColRenderer::SetPlayer(Player * player)
{
	mPlayer = player;
}

void TileColRenderer::SetActor(Monster * actor)
{
	 mMyActor = actor; 
}



void TileColRenderer::Render()
{
	
}


void TileColRenderer::DebugRender()
{
	// m_TilePosSize 

	MyVector2 renderPos = CalPos();

	MyRect rc;

	mRect = mImage->GetRect(0);

	rc.Size = { HALF_TILE_WIDTH, HALF_TILE_HEIGHT };

	rc.Pos.X = (mTileIndex.X * QUARTER_TILE_WIDTH) + (mTileIndex.Y * -QUARTER_TILE_WIDTH) + TILE_SUBOFFSETX - 32;
	rc.Pos.Y = (mTileIndex.X * QUARTER_TILE_HEIGHT) + (mTileIndex.Y * QUARTER_TILE_HEIGHT) + TILE_SUBOFFSETY - 10 - 64 - 32;
	rc.Pos.X += renderPos.IntX();
	rc.Pos.Y += renderPos.IntY();

	mPos = rc.Pos;

	if (false == mIsMovable)
	{
		Image(L"WallTile.bmp");
	}
	else if (nullptr != mMyActor)
	{
		Image(L"ActorTile.bmp");
	}
	else
	{
		Image(L"TileDebug.bmp");
	}

	TransparentBlt(MyTextureManager::BackDC()
		, rc.Pos.IntX()
		, rc.Pos.IntY()
		, rc.Size.IntX()
		, rc.Size.IntY()
		, mImage->ImageHDC()
		, mRect.Pos.IntX() // 이미지의 x에서부터
		, mRect.Pos.IntY() // 이미지의 y에서부터
		, mRect.Size.IntX()  // 여기까지의 x 픽셀
		, mRect.Size.IntY()
		, mTransColor
	);


}


void TileColRenderer::Image(const wchar_t * name, int index)
{
	MyRenderer::Image(name, index);
}

void TileColRenderer::TileDebugSwitch()
{
}
