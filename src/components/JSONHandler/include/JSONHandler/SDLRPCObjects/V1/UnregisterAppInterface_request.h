//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPC_UNREGISTERAPPINTERFACE_REQUEST_INCLUDE
#define NSSMARTDEVICELINKKRPC_UNREGISTERAPPINTERFACE_REQUEST_INCLUDE


#include "JSONHandler/SDLRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPC
{

/**
     Closes an interface from a mobile application.
     After unregisterAppInterface, no commands other than registerAppInterface will be accepted/executed.
     Will fail, if no registerAppInterface was completed successfully before.
*/

  class UnregisterAppInterface_request : public SDLRPCRequest
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
