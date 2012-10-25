#ifndef DELETEINTERACTIONCHOICESET_REQUEST_INCLUDE
#define DELETEINTERACTIONCHOICESET_REQUEST_INCLUDE


#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


/**
     Deletes interaction choice set that has been created with "CreateInteractionChoiceSet".
     The interaction may only be deleted when not currently in use by a "performInteraction".
*/

class DeleteInteractionChoiceSet_request : public ALRPCRequest
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

  unsigned int interactionChoiceSetID;	//!<  (0,2000000000)
};

#endif
