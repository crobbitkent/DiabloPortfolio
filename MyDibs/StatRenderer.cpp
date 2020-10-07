#include "StatRenderer.h"

#include <MyActor.h>
#include <MyRenderer.h>
#include <MyTexture.h>
#include <MyTextureManager.h>
#include <MyImage.h>

#include <MyMacro.h>
#include "Values.h"
#include "Player.h"

#include <stdlib.h>

StatRenderer::StatRenderer()
{
	

	Image(L"NewFont.bmp");

	/*if (nullptr == mBackTexture)
	{
		MyRenderer* temp = mBackTile->CreateRenderer<MyRenderer>(LO_TILE);
		temp->SetRenderStyle(RS_BIT);
		temp->SubSize(MyVector2(BACK_TILE_X, BACK_TILE_Y));
		temp->SubPos(MyVector2(0.f - 18.f, BACK_TILE_Y / 2.f - 166));

		mBackTexture = MyTextureManager::Inst().CreateTexture(L"Stats", BACK_TILE_X, BACK_TILE_Y);

		MyTextureManager::Inst().CreateImage(L"Stats", 1, 1);

		temp->Image(L"Stats");
	}*/

	TransColor(255, 255, 255);

	space = 12;


}


StatRenderer::~StatRenderer()
{

}

void StatRenderer::Render()
{
	// 항상

	if (true == mPlayer->mIsInvenOpen)
	{
	
	}

	if (true == mPlayer->mIsStatOpen)
	{
		ClassRender();
		StatRender();
	}

	if (true == mPlayer->mIsPoint && false == mPlayer->mIsStatOpen)
	{
		LevelUpRender();
	}

}

void StatRenderer::SetPlayer(Player * player)
{
	mPlayer = player;

	mRed = new MyActor();
	mRed->Scene(mPlayer->Scene());
	mRedRenderer = mRed->CreateRenderer<MyRenderer>(LO_DEBUG);
	mRedRenderer->SubSize({ 14, 14 });
	mRedRenderer->Image(L"NewRedFont.bmp");
	mRedRenderer->TransColor(255, 255, 255);
	mRedRenderer->Off();
}

void StatRenderer::ClassRender()
{
	const wchar_t* temp = L"Warrior";
	
	int left = 194;
	int right = 10;
	int bottom = 21;
	int top = 10;
	int a = 1;
	ImageIndex(22);
	TransparentBlt(MyTextureManager::BackDC()
		, left
		, bottom
		, SubSize().IntX()
		, SubSize().IntY()
		, mImage->ImageHDC()
		, mRect.Pos.IntX()
		, mRect.Pos.IntY()
		, mRect.Size.IntX()
		, mRect.Size.IntY()
		, mTransColor
	);

	ImageIndex(0);
	TransparentBlt(MyTextureManager::BackDC()
		, left + space * a++ + 1
		, bottom
		, SubSize().IntX()
		, SubSize().IntY()
		, mImage->ImageHDC()
		, mRect.Pos.IntX()
		, mRect.Pos.IntY()
		, mRect.Size.IntX()
		, mRect.Size.IntY()
		, mTransColor
	);

	ImageIndex(17);
	TransparentBlt(MyTextureManager::BackDC()
		, left + space * a++ + 1
		, bottom
		, SubSize().IntX()
		, SubSize().IntY()
		, mImage->ImageHDC()
		, mRect.Pos.IntX()
		, mRect.Pos.IntY()
		, mRect.Size.IntX()
		, mRect.Size.IntY()
		, mTransColor
	);

	ImageIndex(17);
	TransparentBlt(MyTextureManager::BackDC()
		, left + space * a++ + 1
		, bottom
		, SubSize().IntX()
		, SubSize().IntY()
		, mImage->ImageHDC()
		, mRect.Pos.IntX()
		, mRect.Pos.IntY()
		, mRect.Size.IntX()
		, mRect.Size.IntY()
		, mTransColor
	);

	ImageIndex(8);
	TransparentBlt(MyTextureManager::BackDC()
		, left + space * a++ - 3 + 1
		, bottom
		, SubSize().IntX()
		, SubSize().IntY()
		, mImage->ImageHDC()
		, mRect.Pos.IntX()
		, mRect.Pos.IntY()
		, mRect.Size.IntX()
		, mRect.Size.IntY()
		, mTransColor
	);

	ImageIndex(14);
	TransparentBlt(MyTextureManager::BackDC()
		, left + space * a++ - 6 + 1
		, bottom
		, SubSize().IntX()
		, SubSize().IntY()
		, mImage->ImageHDC()
		, mRect.Pos.IntX()
		, mRect.Pos.IntY()
		, mRect.Size.IntX()
		, mRect.Size.IntY()
		, mTransColor
	);

	ImageIndex(17);
	TransparentBlt(MyTextureManager::BackDC()
		, left + space * a++ - 6 + 1
		, bottom
		, SubSize().IntX()
		, SubSize().IntY()
		, mImage->ImageHDC()
		, mRect.Pos.IntX()
		, mRect.Pos.IntY()
		, mRect.Size.IntX()
		, mRect.Size.IntY()
		, mTransColor
	);
}

void StatRenderer::StatRender()
{
	wchar_t level[3];
	int pLevel = mPlayer->mData.mLevel;
	_itow_s(pLevel, level, 3, 10);

	int startX = 80;

	// 1자리 수인 경우
	if (pLevel < 10)
	{
		ImageIndex(level[0] - 22);
		TransparentBlt(MyTextureManager::BackDC()
			, startX
			, 58
			, SubSize().IntX()
			, SubSize().IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor
		);
	}
	// 2자리 수인 경우
	else
	{
		ImageIndex(level[0] - 22);
		TransparentBlt(MyTextureManager::BackDC()
			, startX - 5
			, 58
			, SubSize().IntX()
			, SubSize().IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor
		);

		ImageIndex(level[1] - 22);
		TransparentBlt(MyTextureManager::BackDC()
			, startX + 5
			, 58
			, SubSize().IntX()
			, SubSize().IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor
		);
	}

	////////////////////////////////// 경험치
	wchar_t exp[11];

	_itow_s(mPlayer->mData.exp, exp, 11, 10);

	std::wstring ex = exp;

	startX = 250 - 5 * ((int)ex.size() - 1);

	for (int i = 0; i < (int)ex.size(); ++i)
	{
		ImageIndex(ex[i] - 22);
		TransparentBlt(MyTextureManager::BackDC()
			, startX + 10 * i
			, 58
			, SubSize().IntX()
			, SubSize().IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor
		);
	}

	wchar_t nextExp[11];

	_itow_s(mPlayer->mExpVec[pLevel - 1], nextExp, 11, 10);

	ex = nextExp;

	startX = 250 - 5 * ((int)ex.size() - 1);

	for (int i = 0; i < (int)ex.size(); ++i)
	{
		ImageIndex(ex[i] - 22);
		TransparentBlt(MyTextureManager::BackDC()
			, startX + 10 * i
			, 86
			, SubSize().IntX()
			, SubSize().IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor
		);
	}

	////////////////////////////////////////// Stat
	wchar_t str[4];
	std::wstring pStr;
	_itow_s(mPlayer->mStr, str, 4, 10);
	pStr = str;
	startX = 105 - 5 * ((int)pStr.size() - 1);

	for (int i = 0; i < (int)pStr.size(); ++i)
	{
		ImageIndex(pStr[i] - 22);
		TransparentBlt(MyTextureManager::BackDC()
			, startX + 10 * i
			, 143
			, SubSize().IntX()
			, SubSize().IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor
		);
	}

	wchar_t cstr[4];
	std::wstring pcStr;
	_itow_s(mPlayer->mCurStr, cstr, 4, 10);
	pcStr = cstr;
	startX = 152 - 5 * ((int)pcStr.size() - 1);

	for (int i = 0; i < (int)pcStr.size(); ++i)
	{
		ImageIndex(pcStr[i] - 22);
		TransparentBlt(MyTextureManager::BackDC()
			, startX + 10 * i
			, 143
			, SubSize().IntX()
			, SubSize().IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor
		);
	}

	wchar_t mag[4];
	std::wstring pMag;
	_itow_s(mPlayer->mMag, mag, 4, 10);
	pMag = mag;
	startX = 105 - 5 * ((int)pMag.size() - 1);
	for (int i = 0; i < (int)pMag.size(); ++i)
	{
		ImageIndex(pMag[i] - 22);
		TransparentBlt(MyTextureManager::BackDC()
			, startX + 10 * i
			, 171
			, SubSize().IntX()
			, SubSize().IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor
		);
	}
	wchar_t cmag[4];
	std::wstring pcMag;
	_itow_s(mPlayer->mCurMag, cmag, 4, 10);
	pcMag = cmag;
	startX = 152 - 5 * ((int)pcMag.size() - 1);
	for (int i = 0; i < (int)pcMag.size(); ++i)
	{
		ImageIndex(pcMag[i] - 22);
		TransparentBlt(MyTextureManager::BackDC()
			, startX + 10 * i
			, 171
			, SubSize().IntX()
			, SubSize().IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor
		);
	}

	wchar_t dex[4];
	std::wstring pDex;
	_itow_s(mPlayer->mDex, dex, 4, 10);
	pDex = dex;
	startX = 105 - 5 * ((int)pDex.size() - 1);
	for (int i = 0; i < (int)pDex.size(); ++i)
	{
		ImageIndex(pDex[i] - 22);
		TransparentBlt(MyTextureManager::BackDC()
			, startX + 10 * i
			, 199
			, SubSize().IntX()
			, SubSize().IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor
		);
	}
	wchar_t cdex[4];
	std::wstring pcDex;
	_itow_s(mPlayer->mCurDex, cdex, 4, 10);
	pcDex = cdex;
	startX = 152 - 5 * ((int)pcDex.size() - 1);
	for (int i = 0; i < (int)pcDex.size(); ++i)
	{
		ImageIndex(pcDex[i] - 22);
		TransparentBlt(MyTextureManager::BackDC()
			, startX + 10 * i
			, 199
			, SubSize().IntX()
			, SubSize().IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor
		);
	}

	wchar_t vit[4];
	std::wstring pVit;
	_itow_s(mPlayer->mVit, vit, 4, 10);
	pVit = vit;
	startX = 105 - 5 * ((int)pVit.size() - 1);
	for (int i = 0; i < (int)pVit.size(); ++i)
	{
		ImageIndex(pVit[i] - 22);
		TransparentBlt(MyTextureManager::BackDC()
			, startX + 10 * i
			, 199 + 28
			, SubSize().IntX()
			, SubSize().IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor
		);
	}
	wchar_t cvit[4];
	std::wstring pcVit;
	_itow_s(mPlayer->mCurVit, cvit, 4, 10);
	pcVit = cvit;
	startX = 152 - 5 * ((int)pcVit.size() - 1);
	for (int i = 0; i < (int)pcVit.size(); ++i)
	{
		ImageIndex(pcVit[i] - 22);
		TransparentBlt(MyTextureManager::BackDC()
			, startX + 10 * i
			, 199 + 28
			, SubSize().IntX()
			, SubSize().IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor
		);
	}
	wchar_t point[3];
	std::wstring pPoint;
	_itow_s(mPlayer->mPoint, point, 3, 10);
	pPoint = point;
	startX = 105 - 5 * ((int)pPoint.size() - 1);

	for (int i = 0; i < (int)pPoint.size(); ++i)
	{
		ImageIndex(pPoint[i] - 22);
		TransparentBlt(MyTextureManager::BackDC()
			, startX + 10 * i
			, 199 + 28 * 2
			, SubSize().IntX()
			, SubSize().IntY()
			, mRedRenderer->Image()->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor
		);
	}

	/////////////////////////////////////////////////////// HP / MP

	

	wchar_t hp[4];
	std::wstring pHP;
	_itow_s(mPlayer->mCurHP, hp, 4, 10);
	pHP = hp;
	startX = 152 - 5 * ((int)pHP.size() - 1);

	if (mPlayer->mCurHP != mPlayer->mData.mMaxHP)
	{
		for (int i = 0; i < (int)pHP.size(); ++i)
		{
			ImageIndex(pHP[i] - 22);
			TransparentBlt(MyTextureManager::BackDC()
				, startX + 10 * i
				, 199 + 28 * 3 + 9
				, SubSize().IntX()
				, SubSize().IntY()
				, mRedRenderer->Image()->ImageHDC()
				, mRect.Pos.IntX()
				, mRect.Pos.IntY()
				, mRect.Size.IntX()
				, mRect.Size.IntY()
				, mTransColor
			);
		}
	}
	else
	{
		for (int i = 0; i < (int)pHP.size(); ++i)
		{
			ImageIndex(pHP[i] - 22);
			TransparentBlt(MyTextureManager::BackDC()
				, startX + 10 * i
				, 199 + 28 * 3 + 9
				, SubSize().IntX()
				, SubSize().IntY()
				, mImage->ImageHDC()
				, mRect.Pos.IntX()
				, mRect.Pos.IntY()
				, mRect.Size.IntX()
				, mRect.Size.IntY()
				, mTransColor
			);
		}
	}	

	wchar_t maxhp[4];
	std::wstring pmaxHP;

	_itow_s(mPlayer->mData.mMaxHP, maxhp, 4, 10);

	pmaxHP = maxhp;

	startX = 105 - 5 * ((int)pmaxHP.size() - 1);

	for (int i = 0; i < (int)pmaxHP.size(); ++i)
	{
		ImageIndex(pmaxHP[i] - 22);
		TransparentBlt(MyTextureManager::BackDC()
			, startX + 10 * i
			, 199 + 28 * 3 + 9
			, SubSize().IntX()
			, SubSize().IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor
		);
	}

	wchar_t mp[4];
	std::wstring pMP;
	_itow_s(mPlayer->mCurMP, mp, 4, 10);
	pMP = mp;
	startX = 152 - 5 * ((int)pMP.size() - 1);

	if (mPlayer->mCurMP != mPlayer->mData.mMaxMP)
	{
		for (int i = 0; i < (int)pMP.size(); ++i)
		{
			int a = pMP[i] - 22;
			ImageIndex(pMP[i] - 22);
			TransparentBlt(MyTextureManager::BackDC()
				, startX + 10 * i
				, 199 + 28 * 4 + 9
				, SubSize().IntX()
				, SubSize().IntY()
				, mRedRenderer->Image()->ImageHDC()
				, mRect.Pos.IntX()
				, mRect.Pos.IntY()
				, mRect.Size.IntX()
				, mRect.Size.IntY()
				, mTransColor
			);
		}
	}
	else
	{
		for (int i = 0; i < (int)pMP.size(); ++i)
		{
			ImageIndex(pMP[i] - 22);
			TransparentBlt(MyTextureManager::BackDC()
				, startX + 10 * i
				, 199 + 28 * 4 + 9
				, SubSize().IntX()
				, SubSize().IntY()
				, mImage->ImageHDC()
				, mRect.Pos.IntX()
				, mRect.Pos.IntY()
				, mRect.Size.IntX()
				, mRect.Size.IntY()
				, mTransColor
			);
		}
	}


	wchar_t maxmp[4];
	std::wstring pmaxMP;
	_itow_s(mPlayer->mData.mMaxMP, maxmp, 4, 10);
	pmaxMP = maxmp;
	startX = 105 - 5 * ((int)pmaxMP.size() - 1);

	for (int i = 0; i < (int)pmaxMP.size(); ++i)
	{
		ImageIndex(pmaxMP[i] - 22);
		TransparentBlt(MyTextureManager::BackDC()
			, startX + 10 * i
			, 199 + 28 * 4 + 9
			, SubSize().IntX()
			, SubSize().IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor
		);
	}

	wchar_t gold[11];
	std::wstring pGold;
	_itow_s(mPlayer->mData.gold, gold, 11, 10);
	pGold = gold;
	startX = 255 - 5 * ((int)pGold.size() - 1);

	for (int i = 0; i < (int)pGold.size(); ++i)
	{
		ImageIndex(pGold[i] - 22);
		TransparentBlt(MyTextureManager::BackDC()
			, startX + 10 * i
			, 133
			, SubSize().IntX()
			, SubSize().IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor
		);
	}

	wchar_t am[3];
	std::wstring pAm;
	_itow_s(mPlayer->mData.armor, am, 3, 10);
	pAm = am;
	startX = 272 - 5 * ((int)pAm.size() - 1);

	for (int i = 0; i < (int)pAm.size(); ++i)
	{
		ImageIndex(pAm[i] - 22);
		TransparentBlt(MyTextureManager::BackDC()
			, startX + 10 * i
			, 171
			, SubSize().IntX()
			, SubSize().IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor
		);
	}

	wchar_t hit[3];
	std::wstring pHit;
	_itow_s(mPlayer->mData.toHit, hit, 3, 10);
	pHit = hit;
	startX = 267 - 5 * ((int)pHit.size() - 1);

	int i = 0;
	for (; i < (int)pHit.size(); ++i)
	{
		ImageIndex(pHit[i] - 22);
		TransparentBlt(MyTextureManager::BackDC()
			, startX + 10 * i
			, 171 + 28
			, SubSize().IntX()
			, SubSize().IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor
		);
	}

	ImageIndex(36);
	TransparentBlt(MyTextureManager::BackDC()
		, startX + 11 * i
		, 171 + 28
		, SubSize().IntX()
		, SubSize().IntY()
		, mImage->ImageHDC()
		, mRect.Pos.IntX()
		, mRect.Pos.IntY()
		, mRect.Size.IntX()
		, mRect.Size.IntY()
		, mTransColor
	);

	wchar_t atmin[4];
	std::wstring pAtmin;
	_itow_s(mPlayer->mData.attackMin, atmin, 4, 10);
	pAtmin = atmin;
	startX = 260 - 5 * ((int)pAtmin.size() - 1);

	i = 0;
	for (; i < (int)pAtmin.size(); ++i)
	{
		ImageIndex(pAtmin[i] - 22);
		TransparentBlt(MyTextureManager::BackDC()
			, startX + 8 * i
			, 171 + 28 * 2
			, SubSize().IntX()
			, SubSize().IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor
		);
	}

	ImageIndex(37);
	TransparentBlt(MyTextureManager::BackDC()
		, startX + 8 * i
		, 171 + 28 * 2
		, SubSize().IntX()
		, SubSize().IntY()
		, mImage->ImageHDC()
		, mRect.Pos.IntX()
		, mRect.Pos.IntY()
		, mRect.Size.IntX()
		, mRect.Size.IntY()
		, mTransColor
	);

	wchar_t atmax[4];
	std::wstring pAtmax;
	_itow_s(mPlayer->mData.attackMax, atmax, 4, 10);
	pAtmax = atmax;
	startX = 285 - 5 * ((int)pAtmin.size() - 1);

	i = 0;
	for (; i < (int)pAtmax.size(); ++i)
	{
		ImageIndex(pAtmax[i] - 22);
		TransparentBlt(MyTextureManager::BackDC()
			, startX + 8 * i
			, 171 + 28 * 2
			, SubSize().IntX()
			, SubSize().IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, mTransColor
		);
	}


	ImageIndex(26);
	TransparentBlt(MyTextureManager::BackDC()
		, 265
		, 180 + 28 * 3
		, SubSize().IntX()
		, SubSize().IntY()
		, mImage->ImageHDC()
		, mRect.Pos.IntX()
		, mRect.Pos.IntY()
		, mRect.Size.IntX()
		, mRect.Size.IntY()
		, mTransColor
	);
	ImageIndex(36);
	TransparentBlt(MyTextureManager::BackDC()
		, 265 + 14
		, 180 + 28 * 3
		, SubSize().IntX()
		, SubSize().IntY()
		, mImage->ImageHDC()
		, mRect.Pos.IntX()
		, mRect.Pos.IntY()
		, mRect.Size.IntX()
		, mRect.Size.IntY()
		, mTransColor
	);

	ImageIndex(26);
	TransparentBlt(MyTextureManager::BackDC()
		, 265
		, 180 + 28 * 4
		, SubSize().IntX()
		, SubSize().IntY()
		, mImage->ImageHDC()
		, mRect.Pos.IntX()
		, mRect.Pos.IntY()
		, mRect.Size.IntX()
		, mRect.Size.IntY()
		, mTransColor
	);
	ImageIndex(36);
	TransparentBlt(MyTextureManager::BackDC()
		, 265 + 14
		, 180 + 28 * 4
		, SubSize().IntX()
		, SubSize().IntY()
		, mImage->ImageHDC()
		, mRect.Pos.IntX()
		, mRect.Pos.IntY()
		, mRect.Size.IntX()
		, mRect.Size.IntY()
		, mTransColor
	);
	ImageIndex(26);
	TransparentBlt(MyTextureManager::BackDC()
		, 265
		, 180 + 28 * 5
		, SubSize().IntX()
		, SubSize().IntY()
		, mImage->ImageHDC()
		, mRect.Pos.IntX()
		, mRect.Pos.IntY()
		, mRect.Size.IntX()
		, mRect.Size.IntY()
		, mTransColor
	);
	ImageIndex(36);
	TransparentBlt(MyTextureManager::BackDC()
		, 265 + 14
		, 180 + 28 * 5
		, SubSize().IntX()
		, SubSize().IntY()
		, mImage->ImageHDC()
		, mRect.Pos.IntX()
		, mRect.Pos.IntY()
		, mRect.Size.IntX()
		, mRect.Size.IntY()
		, mTransColor
	);

}

void StatRenderer::LevelUpRender()
{
	int start = 20;
	ImageIndex(11);
	TransparentBlt(MyTextureManager::BackDC()
		, start
		, 308
		, SubSize().IntX()
		, SubSize().IntY()
		, mImage->ImageHDC()
		, mRect.Pos.IntX()
		, mRect.Pos.IntY()
		, mRect.Size.IntX()
		, mRect.Size.IntY()
		, mTransColor
	);
	ImageIndex(4);
	TransparentBlt(MyTextureManager::BackDC()
		, start + 12
		, 308
		, SubSize().IntX()
		, SubSize().IntY()
		, mImage->ImageHDC()
		, mRect.Pos.IntX()
		, mRect.Pos.IntY()
		, mRect.Size.IntX()
		, mRect.Size.IntY()
		, mTransColor
	);

	ImageIndex(21);
	TransparentBlt(MyTextureManager::BackDC()
		, start + 12 * 2
		, 308
		, SubSize().IntX()
		, SubSize().IntY()
		, mImage->ImageHDC()
		, mRect.Pos.IntX()
		, mRect.Pos.IntY()
		, mRect.Size.IntX()
		, mRect.Size.IntY()
		, mTransColor
	);
	ImageIndex(4);
	TransparentBlt(MyTextureManager::BackDC()
		, start + 12 * 3
		, 308
		, SubSize().IntX()
		, SubSize().IntY()
		, mImage->ImageHDC()
		, mRect.Pos.IntX()
		, mRect.Pos.IntY()
		, mRect.Size.IntX()
		, mRect.Size.IntY()
		, mTransColor
	);
	ImageIndex(11);
	TransparentBlt(MyTextureManager::BackDC()
		, start + 12 * 4
		, 308
		, SubSize().IntX()
		, SubSize().IntY()
		, mImage->ImageHDC()
		, mRect.Pos.IntX()
		, mRect.Pos.IntY()
		, mRect.Size.IntX()
		, mRect.Size.IntY()
		, mTransColor
	);

	ImageIndex(20);
	TransparentBlt(MyTextureManager::BackDC()
		, start + 12 * 6 - 6
		, 308
		, SubSize().IntX()
		, SubSize().IntY()
		, mImage->ImageHDC()
		, mRect.Pos.IntX()
		, mRect.Pos.IntY()
		, mRect.Size.IntX()
		, mRect.Size.IntY()
		, mTransColor
	);

	ImageIndex(15);
	TransparentBlt(MyTextureManager::BackDC()
		, start + 12 * 7 - 6
		, 308
		, SubSize().IntX()
		, SubSize().IntY()
		, mImage->ImageHDC()
		, mRect.Pos.IntX()
		, mRect.Pos.IntY()
		, mRect.Size.IntX()
		, mRect.Size.IntY()
		, mTransColor
	);
}


//
//MyImage* image = MyTextureManager::Inst().FindImage(L"ChurchDungeonTile.bmp");
//
//MyRect rc = image->GetRect(index);
//
//// 타일 위치 설정
//float offsetX = (TILE_OFFSETX) / 2.f;
//float offsetY = offsetX / 2.f;
//
//float startX = MAPSIZE_X / 2.f - offsetX - (y * offsetX);
//float startY = y * offsetY;
//
//float newX = (startX + x * offsetX);
//float newY = startY + (x * offsetY);
//
//MyVector2 pos = { newX + BACK_TILE_X / 2, newY };
//MyVector2 size = { TILE_WIDTH, TILE_HEIGHT };
//
//int transColor = MyGraphic::ColorToInt(255, 255, 255, 0);
//
//TransparentBlt(mBackTexture->TextureDC()
//	, pos.IntX()
//	, pos.IntY()
//	, size.IntX()
//	, size.IntY()
//	, image->ImageHDC()
//	, rc.Pos.IntX()
//	, rc.Pos.IntY()
//	, rc.Size.IntX()
//	, rc.Size.IntY()
//	, transColor
//);
//

