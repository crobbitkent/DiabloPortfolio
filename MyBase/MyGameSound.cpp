#include "MyGameSound.h"
#include "MyMacro.h"
#include "MyDirectory.h"
#include <atlstr.h>
//
#ifdef RELEASEMODE
#ifdef X32
#pragma comment(lib, "fmod_vc")
#else X64
#pragma comment(lib, "fmod_vc")
#endif
#else DEBUGMODE
#ifdef X32
#pragma comment(lib, "fmodL_vc")
#else X64
#pragma comment(lib, "fmodL_vc")
#endif
#endif

FMOD::System* MyGameSound::mSoundSystem;
std::map<std::wstring, MyGameSound*> MyGameSound::mAllSound;
std::list<MyGameSound::MyGameSoundStream*> MyGameSound::mAllStream;
std::list<MyGameSound::MyGameSoundStream*> MyGameSound::mActiveStream;


void MyGameSound::MyGameSoundStream::Init(FMOD::Channel* _Ch)
{
	mIsActive = true;
	mChannel = _Ch;
}

void MyGameSound::MyGameSoundStream::Stop()
{
	mChannel->stop();
	mIsActive = false;

	// 값을 넣어줘서 안에서 검색이 일어나게 되서.
	// 일반적인 함수보다는 느릴것이다.
	mActiveStream.remove(this);
	mAllStream.push_back(this);
}

void MyGameSound::MyGameSoundStream::Paused()
{
	mChannel->setPaused(true);
}

void MyGameSound::MyGameSoundStream::ReStart()
{
	mChannel->setPaused(false);
}

void MyGameSound::MyGameSoundStream::PlayOnOff()
{
	bool Check;

	mChannel->getPaused(&Check);

	if (false == Check)
	{
		Paused();
	}
	else {
		ReStart();
	}
}

MyGameSound::MyGameSoundStream* MyGameSound::PopSoundStream()
{


	if (0 == mAllStream.size())
	{
		return nullptr;
	}

	MyGameSound::MyGameSoundStream* pHGSS = mAllStream.front();
	mAllStream.pop_front();
	return pHGSS;
}

MyGameSound::MyGameSoundStream::MyGameSoundStream() : mIsActive(false)
{
}

MyGameSound::MyGameSoundStream::~MyGameSoundStream()
{
}

void MyGameSound::SoundInit(int Count /*= 100*/)
{
	FMOD::System_Create(&mSoundSystem);

	// 사운드 드라이버와 연결되었다.
	if (nullptr == mSoundSystem)
	{
		BOOM;
	}

	if (FMOD_OK != mSoundSystem->init(32, FMOD_DEFAULT, nullptr))
	{
		BOOM;
	}

	for (int i = 0; i < 5000; i++)
	{
		mAllStream.push_back(new MyGameSoundStream());
	}


	// 
}

void MyGameSound::SoundEnd() {


	//std::map<std::wstring, MyGameSound*>::iterator Start = m_AllSound.begin();
	//std::map<std::wstring, MyGameSound*>::iterator End = m_AllSound.end();
	//for (size_t i = 0; i < length; i++)
	//{
	//}

	while (0 < mAllSound.size())
	{
		if (nullptr != mAllSound.begin()->second)
		{
			delete mAllSound.begin()->second;
			mAllSound.erase(mAllSound.begin());
		}
	}

	for (MyGameSoundStream* HSS : mAllStream)
	{
		delete HSS;
	}

	for (MyGameSoundStream* HSS : mActiveStream)
	{
		delete HSS;
	}

}

void MyGameSound::SoundUpdate()
{
	mSoundSystem->update();
}
void MyGameSound::SoundDestroy()
{
}

void MyGameSound::SoundLoad(const MyFile& _File)
{
	if (mAllSound.end() != mAllSound.find(_File.FileName()))
	{
		BOOM;
		return;
	}

	MyGameSound* NewSound = new MyGameSound();

	if (false == NewSound->Load(_File.GetPath().c_str()))
	{
		BOOM;
	}
	mAllSound.insert(std::map<std::wstring, MyGameSound*>::value_type(_File.FileName(), NewSound));
}

MyGameSound::MyGameSoundStream* MyGameSound::SoundPlay(const wchar_t* _Name, int _Loop)
{
	std::map<std::wstring, MyGameSound*>::iterator FindSoundIter = mAllSound.find(_Name);

	if (mAllSound.end() == FindSoundIter)
	{
		BOOM;
		return nullptr;
	}

	MyGameSoundStream* pStream = PopSoundStream();

	if (nullptr == pStream)
	{
		return nullptr;
	}

	// 재생스트림을 넘겨준다.
	// 진행중인 어떠어떠한 것.
	// CH->setLoopCount(_Loop);
	mSoundSystem->playSound(FindSoundIter->second->mSound, nullptr, false, &pStream->mChannel);
	FindSoundIter->second->mSound->setLoopCount(_Loop);

	if (_Loop == 0)
	{
		BOOM;
	}

	if (1 != _Loop)
	{
		pStream->mChannel->setMode(FMOD_LOOP_NORMAL);
		pStream->mChannel->setLoopCount(_Loop);
	}

	mActiveStream.push_back(pStream);

	return pStream;
	/* CH->setLoopPoints()

	 CH->getPosition 현재 어디까지 재생됐냐.
	 CH->getPitch 속력*/
}

void MyGameSound::SoundPlayToInst(const wchar_t* _Name, int _Loop) {
	std::map<std::wstring, MyGameSound*>::iterator FindSoundIter = mAllSound.find(_Name);

	if (mAllSound.end() == FindSoundIter)
	{
		BOOM;
		return;
	}

	// 진행중인 어떠어떠한 것.
	// CH->setLoopCount(_Loop);
	mSoundSystem->playSound(FindSoundIter->second->mSound, nullptr, false, nullptr);
	FindSoundIter->second->mSound->setLoopCount(_Loop);

	return;
}

////////////////////////////////////////////// MEMBER

MyGameSound::MyGameSound()
{
}


MyGameSound::~MyGameSound()
{
	// 주면 공짜는 없다.
	if (nullptr != mSound)
	{
		mSound->release();
	}
}

bool MyGameSound::Load(const wchar_t* _Path)
{
	// m_pSound->getLength()

	if (FMOD_OK != mSoundSystem->createSound(CW2A(_Path, CP_UTF8).m_psz, FMOD_DEFAULT, nullptr, &mSound))
	{
		BOOM;
		return false;
	}

	return true;
}