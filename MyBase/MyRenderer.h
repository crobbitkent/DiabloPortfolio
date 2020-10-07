#pragma once
#include "MySubActor.h"
#include "MyScene.h"
#include "MyGraphic.h"

enum RENDER_STYLE
{
	RS_NONE,
	RS_TRANS,
	RS_ALPHA,
	RS_BIT,
	RS_HP,
	RS_MP,
	RS_MAX
};

class MyActor;
class MyScene;
class MyImage;

// Actor�� �����ϸ� �ݵ�� �����ڿ� MyRenderer�� �Ҵ����־�� �Ѵ�.
// Renderer�� ����� Scene���� ������ ����/�Ҹ��� Actor�� ���
// Actor�� CreateRenderer���� �����ȴ�.
class MyRenderer : public MySubActor
{
	friend MyActor;
	friend MyScene;



public:
	LEAGUE_ORDER Order() { return mOrder; }

	void ImageIndex(int index);
	int ImageIndex() { return mImageIndex; }

	virtual void Image(const wchar_t* name, int index = 0);
	MyImage* Image() { return mImage; }

	void TransColor(int r, int g, int b)
	{
		mTransColor = MyGraphic::ColorToInt(r, g, b, 0);
	}
	int TransColor() { return mTransColor; }


	void ImageOffset(int x, int y);

	void SetBF(int bf);
	BLENDFUNCTION GetBF() { return BlendFunction; }
	void SetRenderStyle(RENDER_STYLE rs);

	MyRect GetRect() { return mRect; }

	void SetHPRate(float rate) { mHPRate = rate; }
	void SetMPRate(float rate) { mMPRate = rate; }
protected:
	MyRenderer();
	~MyRenderer();
	virtual void Render();
	virtual void DebugRender() {}


protected:
	LEAGUE_ORDER mOrder;
	MyImage* mImage;
	int mImageIndex;
	MyRect mRect;
	int mTransColor;

	BLENDFUNCTION BlendFunction;

private:
	RENDER_STYLE mStyle;
	float mHPRate;
	float mMPRate;
};

