#include "FogRenderer.h"

#include <MyTextureManager.h>
#include <MyImage.h>

#include "Player.h"

FogRenderer::FogRenderer()
{
	Image(L"TileBlack.bmp");

	mRadius = 23;

	mAllFog1.reserve(mRadius * mRadius);
}

FogRenderer::~FogRenderer()
{
	for (size_t i = 0; i < mAllFog1.size(); ++i)
	{
		delete mAllFog1[i];
	}

	mAllFog1.clear();
}

void FogRenderer::Init(Player* player)
{
	// mAllFog.reserve(100);

	mPlayer = player;

	int center = (int)(mRadius / 2);

	TileIndex playerIndex = { center, center };
	TileIndex dis;

	int gap;

	int increase = 30;
	int base = 40;
	int index = 1;

	for (int i = 0; i < mRadius; ++i)
	{
		for (int j = 0; j < mRadius; ++j)
		{
			FogData* temp = new FogData();

			temp->mFogIndex.X = j;
			temp->mFogIndex.Y = i;

			temp->mPos = mPlayer->CalPos();
			temp->mSize = {HALF_TILE_WIDTH, HALF_TILE_HEIGHT};

			//rc.Pos.X = (mTileIndex.X * QUARTER_TILE_WIDTH) + (mTileIndex.Y * -QUARTER_TILE_WIDTH) + TILE_SUBOFFSETX - 32;
			//rc.Pos.Y = (mTileIndTIex.X * QUARTER_TILE_HEIGHT) + (mTileIndex.Y * QUARTER_TILE_HEIGHT) + TILE_SUBOFFSETY - 10 - 64 - 32;

			temp->mPos.X += (j * QUARTER_TILE_WIDTH) + (i * -QUARTER_TILE_WIDTH) + TILE_SUBOFFSETX - 32;
			temp->mPos.Y += (j * QUARTER_TILE_HEIGHT) + (i * QUARTER_TILE_HEIGHT) + TILE_SUBOFFSETY - 10 - 64 + 16 - (mRadius * QUARTER_TILE_HEIGHT);

			dis = temp->mFogIndex - playerIndex;

			dis.X = abs(dis.X);
			dis.Y = abs(dis.Y);

			gap = dis.X + dis.Y;

			if (1 >= gap)
			{

			}
			else if (2 == gap)
			{
				temp->mBF = base;
			}
			else if (3 == gap)
			{
				temp->mBF = base + increase * 1;
			}
			else if (4 == gap)
			{
				temp->mBF = base + increase * 2;
			}
			else if (5 == gap)
			{
				temp->mBF = base + increase * 3;
			}
			else if (6 == gap)
			{
				temp->mBF = base + (int)(increase * 3.7);
			}
			else if (7 == gap)
			{
				temp->mBF = base + (int)(increase * 4.5);
			}
			else if (8 == gap)
			{
				temp->mBF = base + (int)increase * 5;
			}
			else if (9 == gap)
			{
				temp->mBF = base + (int)(increase * 5.5);
			}
			else if (10 == gap)
			{
				temp->mBF = base + (int)(increase * 5.8);
			}
			else if (11 == gap)
			{
				temp->mBF = base + increase * 6;
			}
			else if (12 == gap)
			{
				temp->mBF = base + (int)(increase * 6.2);
			}
			else if (13 == gap)
			{
				temp->mBF = base + (int)(increase * 6.4);
			}
			else if (14 == gap)
			{
				temp->mBF = base + (int)(increase * 6.6);
			}
			else if (15 == gap)
			{
				temp->mBF = base + (int)(increase * 6.8);
			}
			else if (16 == gap)
			{
				temp->mBF = base + increase * 7;
			}
			else
			{

			}

			mAllFog1.push_back(temp);		
		}		
	}
}

void FogRenderer::Render()
{
	for (size_t i = 0; i < mAllFog1.size(); ++i)
	{
		SetBF(mAllFog1[i]->mBF);

		if (true == mPlayer->mIsInvenOpen && false == mPlayer->mIsStatOpen)
		{
			AlphaBlend(MyTextureManager::BackDC()
				, mAllFog1[i]->mPos.IntX() - 160
				, mAllFog1[i]->mPos.IntY()
				, mAllFog1[i]->mSize.IntX()
				, mAllFog1[i]->mSize.IntY()
				, mImage->ImageHDC()
				, mRect.Pos.IntX() // 이미지의 x에서부터
				, mRect.Pos.IntY() // 이미지의 y에서부터
				, mRect.Size.IntX()  // 여기까지의 x 픽셀
				, mRect.Size.IntY()
				, GetBF()
			);
		}
		else if (false == mPlayer->mIsInvenOpen && true == mPlayer->mIsStatOpen)
		{
			AlphaBlend(MyTextureManager::BackDC()
				, mAllFog1[i]->mPos.IntX() + 160
				, mAllFog1[i]->mPos.IntY()
				, mAllFog1[i]->mSize.IntX()
				, mAllFog1[i]->mSize.IntY()
				, mImage->ImageHDC()
				, mRect.Pos.IntX() // 이미지의 x에서부터
				, mRect.Pos.IntY() // 이미지의 y에서부터
				, mRect.Size.IntX()  // 여기까지의 x 픽셀
				, mRect.Size.IntY()
				, GetBF()
			);
		}
		else
		{
			AlphaBlend(MyTextureManager::BackDC()
				, mAllFog1[i]->mPos.IntX()
				, mAllFog1[i]->mPos.IntY()
				, mAllFog1[i]->mSize.IntX()
				, mAllFog1[i]->mSize.IntY()
				, mImage->ImageHDC()
				, mRect.Pos.IntX() // 이미지의 x에서부터
				, mRect.Pos.IntY() // 이미지의 y에서부터
				, mRect.Size.IntX()  // 여기까지의 x 픽셀
				, mRect.Size.IntY()
				, GetBF()
			);
		}	
	}
}
