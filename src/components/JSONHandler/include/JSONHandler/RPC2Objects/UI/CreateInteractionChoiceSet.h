#ifndef RPC2COMMUNICATION_UI_CREATEINTERACTIONCHOICESET_INCLUDE
#define RPC2COMMUNICATION_UI_CREATEINTERACTIONCHOICESET_INCLUDE

#include <vector>
#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/Choice.h"

/*
  interface	RPC2Communication::UI
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

namespace RPC2Communication
{
  namespace UI
  {

    class CreateInteractionChoiceSet : public ::RPC2Communication::RPC2Request
    {
    public:
    
      CreateInteractionChoiceSet(const CreateInteractionChoiceSet& c);
      CreateInteractionChoiceSet(void);
    
      CreateInteractionChoiceSet& operator =(const CreateInteractionChoiceSet&);
    
      virtual ~CreateInteractionChoiceSet(void);
    
      bool checkIntegrity(void);
    
// getters
      unsigned int get_interactionChoiceSetID(void);

      const std::vector< ::AppLinkRPC::Choice>& get_choiceSet(void);


// setters
/// interactionChoiceSetID <= 2000000000
      bool set_interactionChoiceSetID(unsigned int interactionChoiceSetID);

/// 1 <= size <= 100
      bool set_choiceSet(const std::vector< ::AppLinkRPC::Choice>& choiceSet);


    private:

      friend class CreateInteractionChoiceSetMarshaller;

      unsigned int interactionChoiceSetID;
      std::vector< ::AppLinkRPC::Choice> choiceSet;

    };
  }
}

#endif
