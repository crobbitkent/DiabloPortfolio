#pragma once

#include <map>
#include <Windows.h>

#include "MyMath.h"

class MyTexture;
class MyImage;
class MyFile;

// 처음이자 마지막 싱글톤
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

	// 로딩한 이미지
	std::map<std::wstring, MyTexture*> mAllTexture;
	
	// 로딩한 이미지에서 떼어온 이미지
	std::map<std::wstring, MyImage*> mAllImage;

	MyTexture* mBackBufferTexture;


};

