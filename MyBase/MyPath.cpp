#include "MyPath.h"
#include <atlstr.h>
#include "MyFile.h"
#include "MyDirectory.h"
// HANDLE

MyPath::MyPath()
{
}


MyPath::~MyPath()
{
}


void MyPath::PathSetting(const char* _Path) 
{
	PathSetting(CA2W(_Path).m_psz);
}
void MyPath::PathSetting(const wchar_t* _Path) 
{
	mPath = _Path;
}
