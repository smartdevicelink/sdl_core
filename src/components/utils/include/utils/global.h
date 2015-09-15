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

#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_GLOBAL_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_GLOBAL_H_

//#include <log4cxx/logger.h>
#include <string>

#ifdef OS_WINCE
#include <time.h>
#endif
typedef std::basic_string<wchar_t> wchar_string;

#ifdef OS_WIN32
// do nothing
#else
#define CP_UTF8 0
#define CP_ACP 1
#endif

#ifdef OS_WINCE
class Global;
time_t time(time_t* TimeOutPtr);
#endif

class Global
{
public:
	// code & decode
	static bool isUnicode(const void* pBuffer, long size);
	static bool isUtf8(const void* pBuffer, long size);
	static void toUnicode(const char *strSrc, unsigned int nCodePage, wchar_string &wstrOut);
	static void toUnicode(const std::string &strSrc, unsigned int nCodePage, wchar_string &wstrOut);
	static void fromUnicode(const wchar_t *wstrSrc, unsigned int nCodePage, std::string &strOut);
	static void fromUnicode(const wchar_string &wstrSrc, unsigned int nCodePage, std::string &strOut);
	static void anyMultiToUtf8Multi(const std::string &strSrc, std::string &strOut);
	static void utf8MultiToAnsiMulti(const std::string &strSrc, std::string &strOut);
#ifdef OS_WINCE
	// time
	static time_t time(time_t* TimeOutPtr);
#endif
private:
	Global();
	~Global();
};

#endif // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_GLOBAL_H_