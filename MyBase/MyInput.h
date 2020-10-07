#pragma once

#include "MyMacro.h"

#include <Windows.h>
#include <map>
#include <vector>

class MyInput
{
	//////////////////////////// Inner Class //////////////////////////
private:
	class StaticConDes
	{
	public:
		~StaticConDes();
	};

	friend StaticConDes;
	static StaticConDes mConDes;

	class MyKey
	{
		friend MyInput;

	public:
		void Update();
		bool AllKeyCheck();

		void PushKey(int value)
		{
			mKeyVec.push_back(value);
		}

	private:
		std::vector<int> mKeyVec;

		bool mIsUp = true;
		bool mIsDown = false;
		bool mIsPress = false;
		bool mIsRelease = true;
	};


	//////////////////// public static ///////////////////////////////////
public:
	template<typename ...REST>
	static bool CreateKey(const wchar_t* name, REST... value) 
	{
		if (true == IsKey(name))
		{
			BOOM;
			return false;
		}

		TempKey = new MyKey();

		CreateKeyRec(value ...);

		mAllKey.insert(std::map<std::wstring, MyKey*>::value_type(name, TempKey));

		return true;
	}

	static void Update();
	static bool Press(const wchar_t* keyName);
	static bool Down(const wchar_t* keyName);
	static bool Up(const wchar_t* keyName);
	static bool Release(const wchar_t* keyName);

	//////////////////// private static ///////////////////////////////////
private:
	template<typename ...REST>
	static void CreateKeyRec(int pushValue, REST... value)
	{
		TempKey->mKeyVec.push_back(pushValue);
		CreateKeyRec(value ...);
	}

	static void CreateKeyRec()
	{
	}
		

private:
	MyInput();
	~MyInput();



	static MyKey* FindKey(const wchar_t* name);
	static bool IsKey(const wchar_t* name);

public:
private:
	static std::map<std::wstring, MyKey*> mAllKey;
	static MyKey* TempKey;

	static std::map<std::wstring, MyInput::MyKey*>::iterator mInputIter;
	static std::map<std::wstring, MyInput::MyKey*>::iterator mInputEndIter;

};

