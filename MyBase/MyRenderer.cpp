#include "MyRenderer.h"

#include "MyTextureManager.h"
#include "MyActor.h"
#include "MyMacro.h"
#include "MyImage.h"



MyRenderer::MyRenderer()
{
	mHPRate = 0.f;
	mMPRate = 0.f;

	TransColor(255, 0, 255);

	BlendFunction.BlendOp = AC_SRC_OVER;
	BlendFunction.BlendFlags = 0;
	BlendFunction.SourceConstantAlpha = 255;
	BlendFunction.AlphaFormat = AC_SRC_ALPHA;

	// mStyle = RS_TRANS;
}


MyRenderer::~MyRenderer()
{
}

void MyRenderer::Render()
{
	if (nullptr == mActor)
	{
		BOOM;
	}

	MyRect rc = CalRect();

	/*TransparentBlt(MyTextureManager::BackDC()
		, rc.IntLeft()
		, rc.IntTop()
		, rc.Size.IntX()
		, rc.Size.IntY()
		, mImage->ImageHDC()
		, mRect.Pos.IntX()
		, mRect.Pos.IntY()
		, mRect.Size.IntX()
		, mRect.Size.IntY()
		, mTransColor
	);*/


	switch (mStyle)
	{
	case RS_NONE:
		break;
	case RS_TRANS:
		TransparentBlt(MyTextureManager::BackDC()
			, rc.IntLeft()
			, rc.IntTop()
			, rc.Size.IntX()
			, rc.Size.IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY() 
			, mTransColor
		);
		break;
	case RS_ALPHA:
		AlphaBlend(MyTextureManager::BackDC()
			, rc.IntLeft()
			, rc.IntTop()
			, rc.Size.IntX()
			, rc.Size.IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, mRect.Size.IntX()
			, mRect.Size.IntY()
			, BlendFunction
		);
		break;
	case RS_BIT:
		BitBlt(MyTextureManager::BackDC()
			, rc.IntLeft()
			, rc.IntTop()
			, rc.Size.IntX()
			, rc.Size.IntY()
			, mImage->ImageHDC()
			, mRect.Pos.IntX()
			, mRect.Pos.IntY()
			, SRCCOPY
		);
		break;
	case RS_HP:
		TransparentBlt(MyTextureManager::BackDC()
			, 0
			, (int)(345 + 85 * (mHPRate)  )
			, (int)(640					  )
			, (int)(85 - 85 * (mHPRate)	  )
			, mImage->ImageHDC()	 
			, (int)(0 					  )
			, (int)(0 + 85 * (mHPRate)	  )
			, (int)(640					  )
			, (int)(85 - 85 * (mHPRate)	  )
			, mTransColor
		);
		break;
	case RS_MP:
		TransparentBlt(MyTextureManager::BackDC()
			, 0
			, (int)(345 + 85 * (mMPRate)	 )
			, (int)(640						 )
			, (int)(85 - 85 * (mMPRate)		 )
			, mImage->ImageHDC()		
			, (int)(0						 )
			, (int)(0 + 85 * (mMPRate)		 )
			, (int)(640						 )
			, (int)(85 - 85 * (mMPRate)		 )
			, mTransColor
		);
		break;
	case RS_MAX:
		break;
	default:
		break;
	}
}

void MyRenderer::SetRenderStyle(RENDER_STYLE rs)
{
	mStyle = rs;
}

void MyRenderer::SetBF(int bf)
{
	int temp = bf;

	if (bf > 255)
	{
		temp = 255;
	}

	BlendFunction.SourceConstantAlpha = temp;
}

void MyRenderer::Image(const wchar_t * name, int index /*= 0*/)
{
	mImage = MyTextureManager::Inst().FindImage(name);

	ImageIndex(index);

	return;
}

void MyRenderer::ImageIndex(int index)
{
	mImageIndex = index;

	if (nullptr == mImage ||
		index >= mImage->CutCount())
	{
		BOOM;
	}

	mRect = mImage->GetRect(index);
}

void MyRenderer::ImageOffset(int x, int y)
{
	mRect.Pos = mRect.Pos + MyVector2((float)x, (float)y);
}
