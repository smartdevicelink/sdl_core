#ifndef DELETEINTERACTIONCHOICESET_REQUEST_INCLUDE
#define DELETEINTERACTIONCHOICESET_REQUEST_INCLUDE


#include "../../../JSONHandler/ALRPCRequest.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
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
