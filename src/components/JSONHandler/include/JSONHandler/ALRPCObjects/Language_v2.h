#ifndef NSAPPLINKRPC_LANGUAGE_V2_INCLUDE
#define NSAPPLINKRPC_LANGUAGE_V2_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

  class Language_v2
  {
  public:
    enum Language_v2Internal
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
  
    Language_v2() : mInternal(INVALID_ENUM)				{}
    Language_v2(Language_v2Internal e) : mInternal(e)		{}
  
    Language_v2Internal get(void) const	{ return mInternal; }
    void set(Language_v2Internal e)		{ mInternal=e; }
  
  private:
    Language_v2Internal mInternal;
    friend class Language_v2Marshaller;
  };
  
}

#endif
