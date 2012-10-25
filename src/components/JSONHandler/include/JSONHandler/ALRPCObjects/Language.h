#ifndef LANGUAGE_INCLUDE
#define LANGUAGE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


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

#endif
