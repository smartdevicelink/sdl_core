#ifndef ONLANGUAGECHANGE_INCLUDE
#define ONLANGUAGECHANGE_INCLUDE


#include "Language.h"
#include "../../../JSONHandler/ALRPCNotification.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


class OnLanguageChange : public ALRPCNotification
{
public:

  OnLanguageChange(const OnLanguageChange& c);
  OnLanguageChange(void);
  
  virtual ~OnLanguageChange(void);

  bool checkIntegrity(void);

  const Language& get_language(void) const;
  const Language& get_hmiDisplayLanguage(void) const;

  bool set_language(const Language& language_);
  bool set_hmiDisplayLanguage(const Language& hmiDisplayLanguage_);

private:

  friend class OnLanguageChangeMarshaller;

  Language language;
  Language hmiDisplayLanguage;
};

#endif
