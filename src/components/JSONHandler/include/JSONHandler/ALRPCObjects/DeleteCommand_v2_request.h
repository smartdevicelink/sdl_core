#ifndef NSAPPLINKRPC_DELETECOMMAND_V2_REQUEST_INCLUDE
#define NSAPPLINKRPC_DELETECOMMAND_V2_REQUEST_INCLUDE


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

///  Deletes all commands from the in-application menu with the specified command id.

  class DeleteCommand_v2_request : public ALRPCRequest
  {
  public:
  
    DeleteCommand_v2_request(const DeleteCommand_v2_request& c);
    DeleteCommand_v2_request(void);
    
    virtual ~DeleteCommand_v2_request(void);
  
    bool checkIntegrity(void);

    unsigned int get_cmdID(void) const;

    bool set_cmdID(unsigned int cmdID_);

  private:
  
    friend class DeleteCommand_v2_requestMarshaller;


///  ID of the command(s) to delete.
      unsigned int cmdID;	//!<  (0,2000000000)
  };

}

#endif
