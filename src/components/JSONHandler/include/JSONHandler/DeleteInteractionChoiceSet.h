#ifndef DELETEINTERACTIONCHOICESET_CLASS
#define DELETEINTERACTIONCHOICESET_CLASS

#include "JSONHandler/RPC2Request.h"

namespace RPC2Communication
{
    class DeleteInteractionChoiceSet : public RPC2Request
    {
    public:
        DeleteInteractionChoiceSet();
        ~DeleteInteractionChoiceSet();

        void setInteractionChoiceSetId(unsigned int s);
        unsigned int getInteractionChoiceSetId() const;
    
    private:
        unsigned int interactionChoiceSetID;
        friend class DeleteInteractionChoiceSetMarshaller;
    };
}


#endif