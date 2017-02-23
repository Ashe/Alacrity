// by Mark Featherstone (C) 2015 All Rights Reserved.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace std;

#include "UtilsDefines.h"
#include "Error.h"

namespace UDEF
{

char UDEF::g_txt[MAX_TXT];	
char UDEF::g_txt2[MAX_TXT];	
wchar_t UDEF::g_wtxt[MAX_TXT];

const float UDEF::g_PI = 3.1415926535897932384626433832795f;
const float UDEF::g_2PI = 6.283185307179586476925286766559f;
const float MAX_REAL = FLT_MAX;


//convert between 8 and 16bit text
void _convertUTF8toUTF16(utf16char * const pStrTgt, const unsigned int maxTgtSz,
	const utf8char * const pStrSource, const unsigned int maxSrcSz)
{
	MPOD_ASSERT(pStrTgt && pStrSource && maxTgtSz > 0 && maxSrcSz > 0);
	unsigned int lengthWideString = 0;
	lengthWideString = MultiByteToWideChar(CP_UTF8, 0, pStrSource, maxSrcSz, NULL, 0);
	MPOD_ASSERT(lengthWideString < maxTgtSz);
	int ret = MultiByteToWideChar(CP_UTF8, 0, pStrSource, maxSrcSz, pStrTgt, lengthWideString);
	MPOD_ASSERT(ret);
	pStrTgt[ret] = 0;
}

void convertUTF8toUTF16(UDEF::utf16string &strTgt, const UDEF::utf8char *pstrSource, const unsigned int maxChars)
{
	_convertUTF8toUTF16(UDEF::g_wtxt, UDEF::MAX_TXT, pstrSource, maxChars);
	strTgt = g_wtxt;
}

void convertUTF8toUTF16(utf16string &strTgt, const utf8string &strSource)
{
	_convertUTF8toUTF16(UDEF::g_wtxt, UDEF::MAX_TXT, strSource.c_str(), static_cast<unsigned int>(strSource.length()));
	strTgt = g_wtxt;
}

void convertUTF8toUTF16(utf16string &strTgt, const std::vector<utf8char> &strSource)
{
	_convertUTF8toUTF16(UDEF::g_wtxt, UDEF::MAX_TXT, &strSource[0], static_cast<unsigned int>(strSource.size()));
	strTgt = g_wtxt;
}

void convertUTF8toUTF16(vector<utf16char> &strTgt, const vector<utf8char> &strSource)
{
	_convertUTF8toUTF16(&strTgt[0], static_cast<unsigned int>(strTgt.size()), &strSource[0], static_cast<unsigned int>(strSource.size()));
}

void convertUTF8toUTF16(utf16char * const pStrTgt, const unsigned int maxTgtSz, const vector<utf8char> &strSource)
{
	_convertUTF8toUTF16(pStrTgt, maxTgtSz, &strSource[0], static_cast<unsigned int>(strSource.size()));
}

void convertUTF8toUTF16(utf16char * const pStrTgt, const unsigned int maxTgtSz, const utf8string &strSource)
{
	_convertUTF8toUTF16(pStrTgt, maxTgtSz, strSource.c_str(), static_cast<unsigned int>(strSource.length()));
}


void _convertUTF16toUTF8(utf8char * const pStrTgt, const unsigned int maxTgtSz,
	const utf16char * const pStrSource, const unsigned int maxSrcSz)
{
	MPOD_ASSERT(pStrTgt && pStrSource && maxTgtSz > 0 && maxSrcSz > 0);
	unsigned int lengthAnsiString = 0;
	lengthAnsiString = WideCharToMultiByte(CP_UTF8, 0, pStrSource, maxSrcSz, NULL, 0, NULL, NULL);
	MPOD_VERIFY(lengthAnsiString < maxTgtSz);
	int ret = WideCharToMultiByte(CP_UTF8, 0, pStrSource, maxSrcSz, pStrTgt, lengthAnsiString, NULL, NULL);
	MPOD_ASSERT(ret);
	pStrTgt[ret] = 0;
}

void convertUTF16toUTF8(utf8string &strTgt, const utf16char *pstrSource, const unsigned int maxChars)
{
	_convertUTF16toUTF8(UDEF::g_txt, UDEF::MAX_TXT, pstrSource, maxChars);
	strTgt = g_txt;
}

void convertUTF16toUTF8(utf8string &strTgt, const vector<utf16char> &strSource)
{
	_convertUTF16toUTF8(UDEF::g_txt, UDEF::MAX_TXT, &strSource[0], static_cast<unsigned int>(strSource.size()));
	strTgt = g_txt;
}

void convertUTF16toUTF8(utf8string &strTgt, const utf16string &strSource)
{
	_convertUTF16toUTF8(UDEF::g_txt, UDEF::MAX_TXT, strSource.c_str(), static_cast<unsigned int>(strSource.length()));
	strTgt = g_txt;
}

void convertUTF16toUTF8(vector<utf8char> &strTgt, const vector<utf16char> &strSource)
{
	_convertUTF16toUTF8(&strTgt[0], static_cast<unsigned int>(strTgt.size()), &strSource[0], static_cast<unsigned int>(strSource.size()));
}

void convertUTF16toUTF8(utf8char * const pStrTgt, const unsigned int maxTgtSz, const vector<utf16char> &strSource)
{
	_convertUTF16toUTF8(pStrTgt, maxTgtSz, &strSource[0], static_cast<unsigned int>(strSource.size()));
}

void convertUTF16toUTF8(utf8char * const pStrTgt, const unsigned int maxTgtSz, const utf16string &strSource)
{
	_convertUTF16toUTF8(pStrTgt, maxTgtSz, strSource.c_str(), static_cast<unsigned int>(strSource.length()));
}

//a filne name has a path, a name, a name with extennsion and an extension - process these
bool splitFileName(const UDEF::utf8string &fileName, UDEF::utf8string *pNameAndPath, UDEF::utf8string *pExt, UDEF::utf8string *pPath, UDEF::utf8string *pName)
{

	MPOD_ASSERT(pNameAndPath || pExt || pName || pPath);
	if (pNameAndPath)
	{
		string::size_type pos = fileName.find_last_of('.');
		if (pos == string::npos)
			return false;
		*pNameAndPath = fileName.substr(0, pos);
	}
	if (pExt)
	{
		//the last 3 characters
		string::size_type pos = fileName.find_last_of('.');
		if (pos == string::npos)
			return false;
		*pExt = fileName.substr(pos + 1, fileName.length());
	}

	string::size_type lastSlash = fileName.find_last_of('/');
	if (lastSlash == string::npos)
		lastSlash = fileName.find_last_of('\\');
	if (pPath)
	{
		*pPath = "";
		if (lastSlash != string::npos)
			*pPath = fileName.substr(0, lastSlash);
	}
	if (pName)
	{
		if (lastSlash == string::npos)
			lastSlash = -1;

		string::size_type pos = fileName.find_last_of('.');
		if (pos == string::npos)
			pos = fileName.length();
		*pName = fileName.substr(lastSlash + 1, pos);
	}
	return true;
}

}







