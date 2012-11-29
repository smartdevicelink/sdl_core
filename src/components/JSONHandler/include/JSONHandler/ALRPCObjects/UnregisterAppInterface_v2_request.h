#ifndef NSAPPLINKRPC_UNREGISTERAPPINTERFACE_V2_REQUEST_INCLUDE
#define NSAPPLINKRPC_UNREGISTERAPPINTERFACE_V2_REQUEST_INCLUDE


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
     Closes an interface from a mobile application.
     After unregisterAppInterface, no commands other than registerAppInterface will be accepted/executed.
     Will fail, if no registerAppInterface was completed successfully before.
*/

  class UnregisterAppInterface_v2_request : public ALRPCRequest
  {
  public:
  
    UnregisterAppInterface_v2_request(const UnregisterAppInterface_v2_request& c);
    UnregisterAppInterface_v2_request(void);
    
    virtual ~UnregisterAppInterface_v2_request(void);
  
    bool checkIntegrity(void);



  private:
  
    friend class UnregisterAppInterface_v2_requestMarshaller;

  };

}

#endif
