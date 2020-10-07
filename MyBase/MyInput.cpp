#include "MyInput.h"

std::map<std::wstring, MyInput::MyKey*> MyInput::mAllKey;
MyInput::MyKey* MyInput::TempKey;

std::map<std::wstring, MyInput::MyKey*>::iterator MyInput::mInputIter;
std::map<std::wstring, MyInput::MyKey*>::iterator MyInput::mInputEndIter;

///////////////////////////////////// Condes /////////////////////////////
MyInput::StaticConDes MyInput::mConDes;

MyInput::StaticConDes::~StaticConDes()
{
	std::map<std::wstring, MyInput::MyKey*>::iterator StartIter = MyInput::mAllKey.begin();
	std::map<std::wstring, MyInput::MyKey*>::iterator EndIter = MyInput::mAllKey.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (nullptr != StartIter->second)
		{
			delete StartIter->second;
			StartIter->second = nullptr;
		}
	}

	mAllKey.clear();
}
///////////////////////////////////// Condes over /////////////////////////////

void MyInput::Update()
{
	mInputIter = MyInput::mAllKey.begin();
	mInputEndIter = MyInput::mAllKey.end();

	for (; mInputIter != mInputEndIter; ++mInputIter)
	{
		if (nullptr != mInputIter->second)
		{
			mInputIter->second->Update();
		}
	}
}

bool MyInput::Press(const wchar_t * keyName)
{
	MyKey* key = FindKey(keyName);

	if (nullptr == key)
	{
		BOOM;
	}

	return key->mIsPress;
}

bool MyInput::Down(const wchar_t * keyName)
{
	MyKey* key = FindKey(keyName);

	if (nullptr == key)
	{
		BOOM;
	}

	return key->mIsDown;
}

bool MyInput::Up(const wchar_t * keyName)
{
	MyKey* key = FindKey(keyName);

	if (nullptr == key)
	{
		BOOM;
	}

	return key->mIsUp;
}

bool MyInput::Release(const wchar_t * keyName)
{
	MyKey* key = FindKey(keyName);

	if (nullptr == key)
	{
		BOOM;
	}

	return key->mIsRelease;
}

MyInput::MyInput()
{
}


MyInput::~MyInput()
{
}



MyInput::MyKey * MyInput::FindKey(const wchar_t * name)
{
	auto iter = mAllKey.find(name);

	if (iter == mAllKey.end())
	{
		return nullptr;
	}

	return iter->second;
}

bool MyInput::IsKey(const wchar_t * name)
{
	if (mAllKey.end() == mAllKey.find(name))
	{
		return false;
	}

	return true;
}

//////////////////////////// MyKey 구현 부분 ///////////////////////////////////////////

void MyInput::MyKey::Update()
{
	if (true == AllKeyCheck())
	{
		if (true == mIsRelease)
		{
			mIsUp		= false;
			mIsDown		= true;
			mIsPress	= true;
			mIsRelease	= false;
		}
		else if (false == mIsRelease)
		{
			mIsUp		= false;
			mIsDown		= false;
			mIsPress	= true;
			mIsRelease	= false;
		}
	}
	// 키가 아예 안눌린 상황에서
	else
	{
		if (true == mIsDown || true == mIsPress)
		{
			mIsUp		= true;
			mIsDown		= false;
			mIsPress	= false;
			mIsRelease	= true;
		}
		else if(true == mIsUp)
		{
			mIsUp		= false;
			mIsDown		= false;
			mIsPress	= false;
			mIsRelease	= true;
		}
	}
}


bool MyInput::MyKey::AllKeyCheck()
{
	for (size_t i = 0; i < mKeyVec.size(); i++)
	{
		if (0 == GetAsyncKeyState(mKeyVec[i]))
		{
			return false;
		}
	}

	return true;
}
