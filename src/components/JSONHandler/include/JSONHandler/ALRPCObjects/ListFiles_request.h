#ifndef NSAPPLINKRPC_LISTFILES_REQUEST_INCLUDE
#define NSAPPLINKRPC_LISTFILES_REQUEST_INCLUDE


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
     Requests the current list of resident filenames for the registered app
     Not supported on first generation SYNC vehicles. 
*/

  class ListFiles_request : public ALRPCRequest
  {
  public:
  
    ListFiles_request(const ListFiles_request& c);
    ListFiles_request(void);
    
    virtual ~ListFiles_request(void);
  
    bool checkIntegrity(void);



  private:
  
    friend class ListFiles_requestMarshaller;

  };

}

#endif
