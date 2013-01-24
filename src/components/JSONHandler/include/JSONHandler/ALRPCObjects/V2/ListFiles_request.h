#ifndef NSAPPLINKRPCV2_LISTFILES_REQUEST_INCLUDE
#define NSAPPLINKRPCV2_LISTFILES_REQUEST_INCLUDE


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
     Requests the current list of resident filenames for the registered app
     Not supported on first generation SYNC vehicles. 
*/

  class ListFiles_request : public NsAppLinkRPC::ALRPCMessage
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
