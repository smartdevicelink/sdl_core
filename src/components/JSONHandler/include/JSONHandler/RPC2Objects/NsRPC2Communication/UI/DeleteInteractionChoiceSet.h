#ifndef NSRPC2COMMUNICATION_UI_DELETEINTERACTIONCHOICESET_INCLUDE
#define NSRPC2COMMUNICATION_UI_DELETEINTERACTIONCHOICESET_INCLUDE

#include "JSONHandler/RPC2Request.h"


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

    class DeleteInteractionChoiceSet : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      DeleteInteractionChoiceSet(const DeleteInteractionChoiceSet& c);
      DeleteInteractionChoiceSet(void);
    
      DeleteInteractionChoiceSet& operator =(const DeleteInteractionChoiceSet&);
    
      virtual ~DeleteInteractionChoiceSet(void);
    
      bool checkIntegrity(void);
    
// getters
      unsigned int get_interactionChoiceSetID(void);

      int get_appId(void);


// setters
/// interactionChoiceSetID <= 2000000000
      bool set_interactionChoiceSetID(unsigned int interactionChoiceSetID);

      bool set_appId(int appId);


    private:

      friend class DeleteInteractionChoiceSetMarshaller;

      unsigned int interactionChoiceSetID;
      int appId;

    };
  }
}

#endif
