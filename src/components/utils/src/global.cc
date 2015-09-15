/**
* Copyright (c) 2013, Ford Motor Company
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following
* disclaimer in the documentation and/or other materials provided with the
* distribution.
*
* Neither the name of the Ford Motor Company nor the names of its contributors
* may be used to endorse or promote products derived from this software
* without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef MODIFY_FUNCTION_SIGN
#include <global_first.h>
#endif
#ifdef OS_WIN32 
#include <windows.h>
#else
#include <memory.h>
#endif
#include "utils/global.h"

#ifdef OS_WINCE
time_t time(time_t* TimeOutPtr){
	return Global::time(TimeOutPtr);
}
#endif

bool Global::isUnicode(const void* pBuffer, long size){
	unsigned char* first = (unsigned char*)pBuffer;
	unsigned char* second = (unsigned char*)pBuffer + 1;
	if (*first == 0xFF && *second == 0xFE){
		return true;
	}
	else{
		return false;
	}
}

bool Global::isUtf8(const void* pBuffer, long size)
{
	bool IsUTF8 = true;
	unsigned char* start = (unsigned char*)pBuffer;
	unsigned char* end = (unsigned char*)pBuffer + size;
	while (start < end)
	{
		if (*start < 0x80) // (10000000)
		{
			start++;
		}
		else if (*start < (0xC0)) // (11000000)
		{
			IsUTF8 = false;
			break;
		}
		else if (*start < (0xE0)) // (11100000)
		{
			if (start >= end - 1)
				break;
			if ((start[1] & (0xC0)) != 0x80)
			{
				IsUTF8 = false;
				break;
			}
			start += 2;
		}
		else if (*start < (0xF0)) // (11110000)
		{
			if (start >= end - 2)
				break;
			if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80)
			{
				IsUTF8 = false;
				break;
			}
			start += 3;
		}
		else
		{
			IsUTF8 = false;
			break;
		}
	}
	return IsUTF8;
}

void Global::toUnicode(const char *strSrc, unsigned int nCodePage, wchar_string &wstrOut)
{
#ifdef OS_WIN32
	int nLength = MultiByteToWideChar(nCodePage, 0, strSrc, -1, NULL, 0);
	wstrOut.clear();
	wstrOut.resize(nLength);
	MultiByteToWideChar(nCodePage, 0, strSrc, -1, (wchar_t*)&*wstrOut.begin(), nLength);
	wstrOut.erase(--(wstrOut.end())); // remove last '\0'
#endif
}

void Global::toUnicode(const std::string &strSrc, unsigned int nCodePage, wchar_string &wstrOut)
{
#ifdef OS_WIN32
	int nLength = MultiByteToWideChar(nCodePage, 0, strSrc.c_str(), -1, NULL, 0);
	wstrOut.clear();
	wstrOut.resize(nLength);
	MultiByteToWideChar(nCodePage, 0, strSrc.c_str(), -1, (wchar_t*)&*wstrOut.begin(), nLength);
	wstrOut.erase(--(wstrOut.end())); // remove last '\0'
#endif
}

void Global::fromUnicode(const wchar_t *wstrSrc, unsigned int nCodePage, std::string &strOut)
{
#ifdef OS_WIN32
	int nLength = WideCharToMultiByte(nCodePage, 0, wstrSrc, -1, NULL, 0, NULL, NULL);
	strOut.clear();
	strOut.resize(nLength);
	WideCharToMultiByte(nCodePage, 0, wstrSrc, -1, &*strOut.begin(), nLength, NULL, NULL);
	strOut.erase(--(strOut.end())); // remove last '\0'
#endif
}

void Global::fromUnicode(const wchar_string &wstrSrc, unsigned int nCodePage, std::string &strOut)
{
#ifdef OS_WIN32
	int nLength = WideCharToMultiByte(nCodePage, 0, (wchar_t*)wstrSrc.c_str(), -1, NULL, 0, NULL, NULL);
	strOut.clear();
	strOut.resize(nLength);
	WideCharToMultiByte(nCodePage, 0, (wchar_t*)wstrSrc.c_str(), -1, &*strOut.begin(), nLength, NULL, NULL);
	strOut.erase(--(strOut.end())); // remove last '\0'
#endif
}

void Global::anyMultiToUtf8Multi(const std::string &strSrc, std::string &strOut)
{
#ifdef OS_WIN32
	if (isUnicode(strSrc.c_str(), strSrc.size())){
		// muti-unicode to wide-unicode
		wchar_t *pUnicodeData = new wchar_t[strSrc.size()];
		memset(pUnicodeData, 0, strSrc.size());
		int i = 0;
		int j = 0;
		for (i += 2, j = 0; i < strSrc.size(); i += 2, j++){
			pUnicodeData[i] = (strSrc[i + 1]) << 8 | strSrc[i];
		}
		wchar_string strUnicodeData(pUnicodeData);
		// wide-unicode to muti-utf8
		fromUnicode(strUnicodeData, CP_UTF8, strOut);
		delete[] pUnicodeData;
		pUnicodeData = NULL;
	}
	else if (isUtf8(strSrc.c_str(), strSrc.size())){
		strOut = strSrc;
	}
	else{
		// muti-ansi to wide-unicode
		wchar_string strUnicodeData;
		toUnicode(strSrc, CP_ACP, strUnicodeData);
		// wide-unicode to muti-utf8
		fromUnicode(strUnicodeData, CP_UTF8, strOut);
	}
#else
    strOut = strSrc;
#endif
}

void Global::utf8MultiToAnsiMulti(const std::string &strSrc, std::string &strOut)
{
#ifdef OS_WIN32
	wchar_string wc_utf_string;
	toUnicode(strSrc, CP_UTF8, wc_utf_string);
	fromUnicode(wc_utf_string, CP_ACP, strOut);
#else
    strOut = strSrc;
#endif
}

#ifdef OS_WINCE
time_t Global::time(time_t* TimeOutPtr)  
{
	SYSTEMTIME SytemTime;
	FILETIME FileTime;
	time_t Time = 0;

	GetSystemTime( &SytemTime );
	if( SystemTimeToFileTime( &SytemTime, &FileTime ) )
	{
		memcpy( &Time, &FileTime, sizeof( FILETIME ) );
		// subtract the FILETIME value for 1970-01-01 00:00 (UTC)
		Time -= 116444736000000000;
		// convert to seconds
		Time /= 10000000;
	}
	if( TimeOutPtr )
	{
		*TimeOutPtr = Time;
	}
	return Time;
}
#endif

Global::Global()
{
}


Global::~Global()
{
}
