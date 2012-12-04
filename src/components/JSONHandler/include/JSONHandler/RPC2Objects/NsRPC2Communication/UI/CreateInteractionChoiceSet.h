#ifndef NSRPC2COMMUNICATION_UI_CREATEINTERACTIONCHOICESET_INCLUDE
#define NSRPC2COMMUNICATION_UI_CREATEINTERACTIONCHOICESET_INCLUDE

#include <vector>
#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/V1/Choice.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
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

      int get_appId(void);


// setters
/// interactionChoiceSetID <= 2000000000
      bool set_interactionChoiceSetID(unsigned int interactionChoiceSetID);

/// 1 <= size <= 100
      bool set_choiceSet(const std::vector< NsAppLinkRPC::Choice>& choiceSet);

      bool set_appId(int appId);


    private:

      friend class CreateInteractionChoiceSetMarshaller;

      unsigned int interactionChoiceSetID;
      std::vector< NsAppLinkRPC::Choice> choiceSet;
      int appId;

    };
  }
}

#endif
