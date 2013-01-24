#ifndef NSAPPLINKRPCV2_SETDISPLAYLAYOUT_REQUEST_INCLUDE
#define NSAPPLINKRPCV2_SETDISPLAYLAYOUT_REQUEST_INCLUDE

#include <string>

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

/**
     Used to set an alternate display layout.
     If not sent, default screen for given platform will be shown.
*/

  class SetDisplayLayout_request : public NsAppLinkRPC::ALRPCMessage
  {
  public:
  
    SetDisplayLayout_request(const SetDisplayLayout_request& c);
    SetDisplayLayout_request(void);
    
    virtual ~SetDisplayLayout_request(void);
  
    bool checkIntegrity(void);

    const std::string& get_displayLayout(void) const;

    bool set_displayLayout(const std::string& displayLayout_);

  private:
  
    friend class SetDisplayLayout_requestMarshaller;


/**
     Predefined or dynamically created screen layout.
     Currently only predefined screen layouts are defined.
     Predefined layouts include:
       "ONSCREEN_PRESETS"
         Custom screen containing app-defined onscreen presets.  Currently defined for GEN2.
*/
      std::string displayLayout;
  };

}

#endif
