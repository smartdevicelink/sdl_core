#ifndef RPC2COMMUNICATION_UI_DELETEINTERACTIONCHOICESET_INCLUDE
#define RPC2COMMUNICATION_UI_DELETEINTERACTIONCHOICESET_INCLUDE

#include "JSONHandler/RPC2Request.h"


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

    class DeleteInteractionChoiceSet : public ::RPC2Communication::RPC2Request
    {
    public:
    
      DeleteInteractionChoiceSet(const DeleteInteractionChoiceSet& c);
      DeleteInteractionChoiceSet(void);
    
      DeleteInteractionChoiceSet& operator =(const DeleteInteractionChoiceSet&);
    
      virtual ~DeleteInteractionChoiceSet(void);
    
      bool checkIntegrity(void);
    
// getters
      unsigned int get_interactionChoiceSetID(void);


// setters
/// interactionChoiceSetID <= 2000000000
      bool set_interactionChoiceSetID(unsigned int interactionChoiceSetID);


    private:

      friend class DeleteInteractionChoiceSetMarshaller;

      unsigned int interactionChoiceSetID;

    };
  }
}

#endif
