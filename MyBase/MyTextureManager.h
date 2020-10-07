#pragma once

#include <map>
#include <Windows.h>

#include "MyMath.h"

class MyTexture;
class MyImage;
class MyFile;

// ó������ ������ �̱���
class MyTextureManager
{
public:
	/////////////////////// static function /////////////////////////////////////
	static MyTextureManager& Inst()
	{
		static MyTextureManager textureInst;
		return textureInst;
	}
	static const MyVector2& BackDCSize() { return mBackDCSize; }
	static const HDC& BackDC() { return mBackDC; }

	////////////////////// normal public function ///////////////////////////////////
	void Init();
	
	MyTexture* LoadTexture(const MyFile& file);
	MyTexture* CreateTexture(const wchar_t* name, int x, int y);

	MyTexture* FindTexture(const std::wstring& key);


	MyImage* CreateImage(const wchar_t* texName, unsigned int x, unsigned int y);
	MyImage* FindImage(const std::wstring& key);

	

private:
	MyTextureManager();
	~MyTextureManager();

public:
protected:
private:
	// BackBuffer
	static MyVector2 mBackDCSize;
	static HDC mBackDC;

	// �ε��� �̹���
	std::map<std::wstring, MyTexture*> mAllTexture;
	
	// �ε��� �̹������� ����� �̹���
	std::map<std::wstring, MyImage*> mAllImage;

	MyTexture* mBackBufferTexture;


};

