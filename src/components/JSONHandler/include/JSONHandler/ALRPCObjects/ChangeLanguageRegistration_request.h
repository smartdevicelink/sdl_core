#ifndef CHANGELANGUAGEREGISTRATION_REQUEST_INCLUDE
#define CHANGELANGUAGEREGISTRATION_REQUEST_INCLUDE


#include "Language.h"
#include "../../../JSONHandler/ALRPCRequest.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


class ChangeLanguageRegistration_request : public ALRPCRequest
{
public:

  ChangeLanguageRegistration_request(const ChangeLanguageRegistration_request& c);
  ChangeLanguageRegistration_request(void);
  
  virtual ~ChangeLanguageRegistration_request(void);

  ChangeLanguageRegistration_request& operator =(const ChangeLanguageRegistration_request&);

  bool checkIntegrity(void);

  const Language* get_language(void) const;
  const Language* get_hmiDisplayLanguage(void) const;

  void reset_language(void);
  bool set_language(const Language& language_);
  void reset_hmiDisplayLanguage(void);
  bool set_hmiDisplayLanguage(const Language& hmiDisplayLanguage_);

private:

  friend class ChangeLanguageRegistration_requestMarshaller;

  Language* language;
  Language* hmiDisplayLanguage;
};

#endif
