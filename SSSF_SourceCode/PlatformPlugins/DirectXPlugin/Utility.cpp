#include "stdafx.h"
#include "Utility.h"
#include <io.h>

// Output a string to Visual Studio's output pane
void CUtility::DebugString(const wstring &str)
{
	//OutputDebugString(str.c_str());
}

// Check to see if a DirectX call failed. If it did output the error string.
bool CUtility::FailedHr(HRESULT hr)
{
	if (FAILED(hr))
	{
		DebugString(L"DirectX Reported Error: "/*+DXGetErrorString(hr)+ " - " + 
			ToString(DXGetErrorDescription(hr))+"\n"*/);

		return true;
	}

	return false;
}

/**
 * \brief makes a new character string from the passed one
 * \param charString - a zero terminated char string
 * \return the new string. Caller is responsible for freeing the memory
 * \author Keith Ditchburn \date 17 July 2005
*/
char* CUtility::DuplicateCharString(const WCHAR* charString)
{
	wstring s = charString;
    if (!charString)
		return 0;

	size_t len= s.length() + 1;
	char *newString = new char[len];
	memcpy(newString, charString, len*sizeof(char));

	return newString;
}

/* 
Try to find a texture file
There is often pain finding the textures for .x files as sometimes the filename
includes the path and other times it does not. It is a cause of a lot of issues. By
saving your .x file as text you can look at it and see what the filenames are set to
which can help spot problems.
*/
bool CUtility::FindFile(wstring *filename)
{
	if (!filename)
		return false;

	// Look for it first using the original filename and path
	if (DoesFileExist(*filename))
		return true;

	// Next try with stripping the path from it
	wstring pathOnly;
	wstring filenameOnly;
	SplitPath(*filename,&pathOnly,&filenameOnly);

	if (DoesFileExist(filenameOnly))
	{
		*filename=filenameOnly;
		return true;
	}

	// Could now try by looking in a specific data folder....
	return false;
}

/*
	Uses the basic C run time _access fucntion to determine if the file exists
*/
bool CUtility::DoesFileExist(const wstring &filename)
{
	string ns(filename.begin(), filename.end());
	return (_access(ns.c_str(), 0) != -1);
}

/*
	Splits a path and filename string
	pathOnly or filenameOnly can be null if they are not required
*/
void CUtility::SplitPath(const wstring& inputPath, wstring* pathOnly, wstring* filenameOnly)
{
	wstring fullPath(inputPath);
	std::replace(fullPath.begin(),fullPath.end(),'\\','/');

	wstring::size_type lastSlashPos=fullPath.find_last_of('/');

	// check for there being no path element in the input
	if (lastSlashPos==std::string::npos)
	{
		*pathOnly=L"";
		*filenameOnly=fullPath;
	}
	else // do the split
	{
		if (pathOnly)
			*pathOnly=fullPath.substr(0,lastSlashPos);

		if (filenameOnly)
			*filenameOnly=fullPath.substr(lastSlashPos+1,fullPath.size()-lastSlashPos-1);
	}
}

/*
	Retrieves the current directory
	Takes into account buffer size etc.
*/
wstring CUtility::GetTheCurrentDirectory()
{
	int bufferSize=GetCurrentDirectory(0,NULL);
	//char *buffer=new char[bufferSize];
	WCHAR * buffer = new WCHAR[bufferSize];
	GetCurrentDirectory(bufferSize, buffer);
	wstring ws(buffer);
	delete []buffer;

	//std::string s; wstringstream(directory) >> s;
	return  ws;
}