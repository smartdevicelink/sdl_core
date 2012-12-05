#ifndef NSAPPLINKRPCV2_RESETGLOBALPROPERTIES_REQUEST_INCLUDE
#define NSAPPLINKRPCV2_RESETGLOBALPROPERTIES_REQUEST_INCLUDE

#include <vector>

#include "GlobalProperty.h"
#include "JSONHandler/ALRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

///  Allows resetting global properties.

  class ResetGlobalProperties_request : public NsAppLinkRPC::ALRPCMessage
  {
  public:
  
    ResetGlobalProperties_request(const ResetGlobalProperties_request& c);
    ResetGlobalProperties_request(void);
    
    virtual ~ResetGlobalProperties_request(void);
  
    bool checkIntegrity(void);

    const std::vector<GlobalProperty>& get_properties(void) const;

    bool set_properties(const std::vector<GlobalProperty>& properties_);

  private:
  
    friend class ResetGlobalProperties_requestMarshaller;


///  Contains the names of all global properties (like timeoutPrompt) that should be unset. Resetting means, that they have the same value as at start up (default)
      std::vector<GlobalProperty> properties;	//!<   [%s..%s] 
  };

}

#endif
