#ifndef NSAPPLINKRPC_SETDISPLAYLAYOUT_REQUEST_INCLUDE
#define NSAPPLINKRPC_SETDISPLAYLAYOUT_REQUEST_INCLUDE

#include <string>

#include "JSONHandler/ALRPCRequest.h"


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

/**
     Used to set an alternate display layout.
     If not sent, default screen for given platform will be shown.
*/

  class SetDisplayLayout_request : public ALRPCRequest
  {
  public:
  
    SetDisplayLayout_request(const SetDisplayLayout_request& c);
    SetDisplayLayout_request(void);
    
    virtual ~SetDisplayLayout_request(void);
  
    SetDisplayLayout_request& operator =(const SetDisplayLayout_request&);
  
    bool checkIntegrity(void);

    const std::string* get_displayLayout(void) const;

    void reset_displayLayout(void);
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
      std::string* displayLayout;
  };

}

#endif
