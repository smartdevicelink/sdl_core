#ifndef CREATEINTERACTIONCHOICESET_REQUEST_INCLUDE
#define CREATEINTERACTIONCHOICESET_REQUEST_INCLUDE

#include <vector>

#include "Choice.h"
#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Oct 30 08:29:32 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/


///  creates interaction choice set to be used later by performInteraction

class CreateInteractionChoiceSet_request : public ALRPCRequest
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

#endif
