// by Mark Featherstone (C) 2015 All Rights Reserved.
#pragma once
#ifndef UTILSDEFINES_H
#define UTILSDEFINES_H

#include <string>
#include <vector>

/*
Ideally we would use our own defines for all types and then if we need
to change platform and some type will end up not the right size we can
just change it once here and its fixed.

Also includes any magic numbers we want to use.
*/

namespace UDEF
{

const char LINE_FEED = 10; ///<an ascii line feed character code
const char CR_RETURN = 13; ///<carriage return ascii code
const unsigned short UNICODE_HEADER = 0xfeff;
const unsigned int MAX_UINT = 0xffffffff;
const int MAX_INT   = 0x7fffffff;
extern const float MAX_REAL;
const unsigned short MAX_USHORT = 0xffff;
const unsigned char MAX_UCHAR = 0xff;
typedef char utf8char;
typedef wchar_t utf16char;
typedef std::string utf8string;
typedef std::wstring utf16string;
// used when we want to allow a very tiny tolerance around zero
const float EPSILON = 10e-9f;
// used to allow a larger tolerance to floating point comparisons
const float BIG_EPSILON = 10e-7f;
// bigger tolerance again...
const float EVEN_BIGGER_EPSILON = 10e-4f;
// bigger again
const float EVEN_BIGGER_AGAIN_EPSILON = 10e-2f;
// an even bigger tolerance value
const float HUGE_EPSILON = 1.0f;
//a general purpose text buffer size big enough for the biggest path or mssg
const int MAX_TXT = 2048;		

extern const float g_PI;
extern const float g_2PI;
extern char g_txt[MAX_TXT];				//these buffers are not safe between functions!!!
extern char g_txt2[MAX_TXT];			//only to be used as temporary scratch pads to prevent temporary stack allocation
extern wchar_t g_wtxt[MAX_TXT];			//of large plain buffers

//convert to and from degrees and radians
inline float D2R(float x) { return (x * g_PI)/180.0f; }
inline float R2D(float x) { return (x * 180.0f) / g_PI; }

void convertUTF8toUTF16(utf16string &strTgt, const utf8char *pstrSource, const unsigned int maxChars);
void convertUTF8toUTF16(utf16string &strTgt, const utf8string &strSource);
void convertUTF8toUTF16(utf16string &strTgt, const std::vector<utf8char> &strSource);
void convertUTF8toUTF16(std::vector<UDEF::utf16char> &strTgt, const std::vector<UDEF::utf8char> &strSource);
void convertUTF8toUTF16(utf16char * const pStrTgt, const unsigned int maxTgtSz, const std::vector<UDEF::utf8char> &strSource);
void convertUTF8toUTF16(utf16char * const pStrTgt, const unsigned int maxTgtSz, const UDEF::utf8string &strSource);

void convertUTF16toUTF8(utf8string &strTgt, const UDEF::utf16char *pstrSource, const unsigned int maxChars);
void convertUTF16toUTF8(utf8string &strTgt, const std::vector<UDEF::utf16char> &strSource);
void convertUTF16toUTF8(utf8string &strTgt, const UDEF::utf16string &strSource);
void convertUTF16toUTF8(std::vector<UDEF::utf8char> &strTgt, const std::vector<UDEF::utf16char> &strSource);
void convertUTF16toUTF8(utf8char * const pStrTgt, const unsigned int maxTgtSz, const std::vector<UDEF::utf16char> &strSource);
void convertUTF16toUTF8(utf8char * const pStrTgt, const unsigned int maxTgtSz, const UDEF::utf16string &strSource);

//grow buffers, don't keep allocating new ones
template <class T>
void growBuffer(std::vector<T> &buffer, const unsigned int desiredSz, const unsigned int minSz)
{
	unsigned int sz = static_cast<unsigned int>(buffer.size());
	unsigned int newSz = desiredSz;
	if (newSz < desiredSz)
		newSz = desiredSz;
	if (sz < newSz)
		buffer.insert(buffer.end(), newSz - sz, 0);
}


bool splitFileName(const UDEF::utf8string &fileName, UDEF::utf8string *pNameAndPath = NULL, UDEF::utf8string *pExt = NULL,
					UDEF::utf8string *pPath = NULL, UDEF::utf8string *pName = NULL);


}

#endif