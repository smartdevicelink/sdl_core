//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_DELETECOMMAND_REQUEST_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_DELETECOMMAND_REQUEST_INCLUDE


#include "JSONHandler/SDLRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPCV2
{

///  Deletes all commands from the in-application menu with the specified command id.

  class DeleteCommand_request : public NsSmartDeviceLinkRPC::SDLRPCMessage
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
