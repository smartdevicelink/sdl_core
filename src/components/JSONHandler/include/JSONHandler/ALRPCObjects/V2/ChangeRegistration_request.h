#ifndef NSAPPLINKRPCV2_CHANGEREGISTRATION_REQUEST_INCLUDE
#define NSAPPLINKRPCV2_CHANGEREGISTRATION_REQUEST_INCLUDE


#include "Language.h"
#include "JSONHandler/ALRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  class ChangeRegistration_request : public NsAppLinkRPC::ALRPCMessage
  {
  public:
  
    ChangeRegistration_request(const ChangeRegistration_request& c);
    ChangeRegistration_request(void);
    
    virtual ~ChangeRegistration_request(void);
  
    bool checkIntegrity(void);

    const Language& get_language(void) const;
    const Language& get_hmiDisplayLanguage(void) const;

    bool set_language(const Language& language_);
    bool set_hmiDisplayLanguage(const Language& hmiDisplayLanguage_);

  private:
  
    friend class ChangeRegistration_requestMarshaller;


///  Requested SYNC voice engine (VR+TTS) language registration
      Language language;

///  Request display language registration
      Language hmiDisplayLanguage;
  };

}

#endif
