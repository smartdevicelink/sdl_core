//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPC_DELETEINTERACTIONCHOICESET_REQUEST_INCLUDE
#define NSSMARTDEVICELINKKRPC_DELETEINTERACTIONCHOICESET_REQUEST_INCLUDE


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
     Deletes interaction choice set that has been created with "CreateInteractionChoiceSet".
     The interaction may only be deleted when not currently in use by a "performInteraction".
*/

  class DeleteInteractionChoiceSet_request : public SDLRPCRequest
  {
  public:
  
    DeleteInteractionChoiceSet_request(const DeleteInteractionChoiceSet_request& c);
    DeleteInteractionChoiceSet_request(void);
    
    virtual ~DeleteInteractionChoiceSet_request(void);
  
    bool checkIntegrity(void);

    unsigned int get_interactionChoiceSetID(void) const;

    bool set_interactionChoiceSetID(unsigned int interactionChoiceSetID_);

  private:
  
    friend class DeleteInteractionChoiceSet_requestMarshaller;


///  ID of the interaction choice set to delete.
      unsigned int interactionChoiceSetID;	//!<  (0,2000000000)
  };

}

#endif
