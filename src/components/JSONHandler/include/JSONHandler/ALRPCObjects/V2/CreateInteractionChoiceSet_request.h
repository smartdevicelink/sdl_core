#ifndef NSAPPLINKRPCV2_CREATEINTERACTIONCHOICESET_REQUEST_INCLUDE
#define NSAPPLINKRPCV2_CREATEINTERACTIONCHOICESET_REQUEST_INCLUDE

#include <vector>

#include "Choice.h"
#include "JSONHandler/ALRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

///  creates interaction choice set to be used later by performInteraction

  class CreateInteractionChoiceSet_request : public NsAppLinkRPC::ALRPCMessage
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
