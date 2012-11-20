#ifndef NSRPC2COMMUNICATION_UI_CREATEINTERACTIONCHOICESET_INCLUDE
#define NSRPC2COMMUNICATION_UI_CREATEINTERACTIONCHOICESET_INCLUDE

#include <vector>
#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/Choice.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Nov 20 13:32:23 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class CreateInteractionChoiceSet : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      CreateInteractionChoiceSet(const CreateInteractionChoiceSet& c);
      CreateInteractionChoiceSet(void);
    
      CreateInteractionChoiceSet& operator =(const CreateInteractionChoiceSet&);
    
      virtual ~CreateInteractionChoiceSet(void);
    
      bool checkIntegrity(void);
    
// getters
      unsigned int get_interactionChoiceSetID(void);

      const std::vector< NsAppLinkRPC::Choice>& get_choiceSet(void);


// setters
/// interactionChoiceSetID <= 2000000000
      bool set_interactionChoiceSetID(unsigned int interactionChoiceSetID);

/// 1 <= size <= 100
      bool set_choiceSet(const std::vector< NsAppLinkRPC::Choice>& choiceSet);


    private:

      friend class CreateInteractionChoiceSetMarshaller;

      unsigned int interactionChoiceSetID;
      std::vector< NsAppLinkRPC::Choice> choiceSet;

    };
  }
}

#endif
