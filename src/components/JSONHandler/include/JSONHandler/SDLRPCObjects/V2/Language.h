//
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#ifndef NSSMARTDEVICELINKKRPCV2_LANGUAGE_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_LANGUAGE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPCV2
{

  class Language
  {
  public:
    enum LanguageInternal
    {
      INVALID_ENUM=-1,

///  US English
      EN_US=0,

///  Mexican Spanish
      ES_MX=1,

///  Canadian French
      FR_CA=2,

///  EU German
      DE_EU=3,

///  EU Spanish
      ES_EU=4,

///  UK English
      EN_EU=5,

///  Russian
      RU_RU=6,

///  Turkish
      TR_TR=7,

///  EU Polish
      PL_EU=8,

///  French
      FR_EU=9,

///  Italian
      IT_EU=10,

///  Swedish
      SV_EU=11,

///  Portuguese
      PT_EU=12,

///  Dutch (Standard)
      NL_EU=13,

///  Australian English
      EN_AU=14,

///  Chinese (Mandarin)
      ZH_CN=15,

///  Taiwanese (Mandarin)
      ZH_TW=16,

///  Japanese
      JA_JP=17,

///  Arabic
      AR=18,

///  Korean
      KO_KR=19
    };

    Language() : mInternal(INVALID_ENUM)				{}
    Language(LanguageInternal e) : mInternal(e)		{}
    Language& operator=(const Language& lng) { set(lng.get()); return *this; }

    bool operator== (const Language& lng) { return mInternal == lng.get(); }

    LanguageInternal get(void) const	{ return mInternal; }
    void set(LanguageInternal e)		{ mInternal=e; }

  private:
    LanguageInternal mInternal;
    friend class LanguageMarshaller;
  };

}

#endif
