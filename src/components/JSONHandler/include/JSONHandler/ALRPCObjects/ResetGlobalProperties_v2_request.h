#ifndef NSAPPLINKRPC_RESETGLOBALPROPERTIES_V2_REQUEST_INCLUDE
#define NSAPPLINKRPC_RESETGLOBALPROPERTIES_V2_REQUEST_INCLUDE

#include <vector>

#include "GlobalProperty_v2.h"
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

///  Allows resetting global properties.

  class ResetGlobalProperties_v2_request : public ALRPCRequest
  {
  public:
  
    ResetGlobalProperties_v2_request(const ResetGlobalProperties_v2_request& c);
    ResetGlobalProperties_v2_request(void);
    
    virtual ~ResetGlobalProperties_v2_request(void);
  
    bool checkIntegrity(void);

    const std::vector<GlobalProperty_v2>& get_properties(void) const;

    bool set_properties(const std::vector<GlobalProperty_v2>& properties_);

  private:
  
    friend class ResetGlobalProperties_v2_requestMarshaller;


///  Contains the names of all global properties (like timeoutPrompt) that should be unset. Resetting means, that they have the same value as at start up (default)
      std::vector<GlobalProperty_v2> properties;	//!<   [%s..%s] 
  };

}

#endif
