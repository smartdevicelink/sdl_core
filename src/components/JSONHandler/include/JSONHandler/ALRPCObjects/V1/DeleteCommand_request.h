#ifndef NSAPPLINKRPC_DELETECOMMAND_REQUEST_INCLUDE
#define NSAPPLINKRPC_DELETECOMMAND_REQUEST_INCLUDE


#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Dec 13 13:37:09 2012
  source stamp	Thu Dec 13 13:33:23 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

///  Deletes all commands from the in-application menu with the specified command id.

  class DeleteCommand_request : public ALRPCRequest
  {
  public:
  
    DeleteCommand_request(const DeleteCommand_request& c);
    DeleteCommand_request(void);
    
    virtual ~DeleteCommand_request(void);
  
    bool checkIntegrity(void);

    unsigned int get_cmdID(void) const;

    bool set_cmdID(unsigned int cmdID_);

  private:
  
    friend class DeleteCommand_requestMarshaller;


///  ID of the command(s) to delete.
      unsigned int cmdID;	//!<  (0,2000000000)
  };

}

#endif
