#pragma once

#include <stdafx.h>

// Template to convert standard types into strings
template <class T>
std::string ToString(const T & t)
{
	std::ostringstream oss;
	oss.clear();
	oss << t;
	return oss.str();
}

/*
	Some useful functions gathered together into a class that is never instantiated
	I could have used a namespace or simply declared the functions outside of a class
	but I prefer this method using static functions as it gathers them together nicely
*/
class CUtility
{
private:
	~CUtility(void){};
public:
	CUtility(void){};
	
	static bool FailedHr(HRESULT hr);
	static void DebugString(const wstring &str);
	static char* DuplicateCharString(const WCHAR* c_str);
	static bool FindFile(wstring *filename);
	static bool DoesFileExist(const wstring &filename);
	static void SplitPath(const wstring& inputPath, wstring* pathOnly, wstring* filenameOnly);
	static wstring GetTheCurrentDirectory();
};
