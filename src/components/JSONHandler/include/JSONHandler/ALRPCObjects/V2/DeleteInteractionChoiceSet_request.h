#ifndef NSAPPLINKRPCV2_DELETEINTERACTIONCHOICESET_REQUEST_INCLUDE
#define NSAPPLINKRPCV2_DELETEINTERACTIONCHOICESET_REQUEST_INCLUDE


#include "JSONHandler/ALRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

/**
     Deletes interaction choice set that has been created with "CreateInteractionChoiceSet".
     The interaction may only be deleted when not currently in use by a "performInteraction".
*/

  class DeleteInteractionChoiceSet_request : public NsAppLinkRPC::ALRPCMessage
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
