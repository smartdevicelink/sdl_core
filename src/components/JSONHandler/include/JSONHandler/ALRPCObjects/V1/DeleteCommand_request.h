#ifndef NSAPPLINKRPC_DELETECOMMAND_REQUEST_INCLUDE
#define NSAPPLINKRPC_DELETECOMMAND_REQUEST_INCLUDE


#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
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
