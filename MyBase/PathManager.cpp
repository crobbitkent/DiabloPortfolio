#include "PathManager.h"
#include <atlstr.h>
#include "MyFile.h"
#include "MyDirectory.h"
// HANDLE

PathManager::PathManager()
{
}


PathManager::~PathManager()
{
}


void PathManager::PathSetting(const char* _Path) 
{
	PathSetting(CA2W(_Path).m_psz);
}
void PathManager::PathSetting(const wchar_t* _Path) 
{
	mPath = _Path;
}
