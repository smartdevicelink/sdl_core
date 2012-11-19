#ifndef NSAPPLINKRPC_UNREGISTERAPPINTERFACE_REQUEST_INCLUDE
#define NSAPPLINKRPC_UNREGISTERAPPINTERFACE_REQUEST_INCLUDE


#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Mon Nov 19 10:37:06 2012
  source stamp	Mon Nov 19 10:35:56 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

/**
     Closes an interface from a mobile application.
     After unregisterAppInterface, no commands other than registerAppInterface will be accepted/executed.
     Will fail, if no registerAppInterface was completed successfully before.
*/

  class UnregisterAppInterface_request : public ALRPCRequest
  {
  public:
  
    UnregisterAppInterface_request(const UnregisterAppInterface_request& c);
    UnregisterAppInterface_request(void);
    
    virtual ~UnregisterAppInterface_request(void);
  
    bool checkIntegrity(void);



  private:
  
    friend class UnregisterAppInterface_requestMarshaller;

  };

}

#endif
