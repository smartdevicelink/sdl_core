//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPC_CREATEINTERACTIONCHOICESET_REQUEST_INCLUDE
#define NSSMARTDEVICELINKKRPC_CREATEINTERACTIONCHOICESET_REQUEST_INCLUDE

#include <vector>

#include "Choice.h"
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

///  creates interaction choice set to be used later by performInteraction

  class CreateInteractionChoiceSet_request : public SDLRPCRequest
  {
  public:
  
    CreateInteractionChoiceSet_request(const CreateInteractionChoiceSet_request& c);
    CreateInteractionChoiceSet_request(void);
    
    virtual ~CreateInteractionChoiceSet_request(void);
  
    bool checkIntegrity(void);

    unsigned int get_interactionChoiceSetID(void) const;
    const std::vector<Choice>& get_choiceSet(void) const;

    bool set_interactionChoiceSetID(unsigned int interactionChoiceSetID_);
    bool set_choiceSet(const std::vector<Choice>& choiceSet_);

  private:
  
    friend class CreateInteractionChoiceSet_requestMarshaller;


///  Unique ID used for this interaction choice set.
      unsigned int interactionChoiceSetID;	//!<  (0,2000000000)
      std::vector<Choice> choiceSet;	//!<   [%s..%s] 
  };

}

#endif
