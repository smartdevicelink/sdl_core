#ifndef NSAPPLINKRPC_LANGUAGE_INCLUDE
#define NSAPPLINKRPC_LANGUAGE_INCLUDE


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

///  The set of potential languages

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
      FR_CA=2
    };
  
    Language() : mInternal(INVALID_ENUM)				{}
    Language(LanguageInternal e) : mInternal(e)		{}
  
    LanguageInternal get(void) const	{ return mInternal; }
    void set(LanguageInternal e)		{ mInternal=e; }
  
  private:
    LanguageInternal mInternal;
    friend class LanguageMarshaller;
  };
  
}

#endif
