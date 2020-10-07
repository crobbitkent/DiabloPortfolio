#pragma once

#include <vector>
#include "MyTexture.h"

class MyTextureManager;

// �̹��� ����(Texture)���� ����� �ϳ��� �̹���
class MyImage
{
	friend MyTextureManager;

public:
	MyImage();
	~MyImage();

	void Cut(MyTexture* texture, int x, int y);

	MyRect GetRect(int index) { return mAllCutData[index]; }
	HDC ImageHDC() { return mDC; }
	size_t CutCount() { return mAllCutData.size(); }

private:


public:
private:
	MyTexture* mTexture;
	HDC mDC;
	std::vector<MyRect> mAllCutData;

public:
	MyTexture* Texture() {
		return mTexture;
	}
};

