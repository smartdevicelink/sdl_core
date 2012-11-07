#ifndef CREATEINTERACTIONCHOICESET_CLASS
#define CREATEINTERACTIONCHOICESET_CLASS

#include "JSONHandler/RPC2Request.h"
#include "JSONHandler/ALRPCObjects/Choice.h"
#include <vector>

namespace RPC2Communication
{
    class CreateInteractionChoiceSet : public RPC2Request
    {
    public:
        CreateInteractionChoiceSet();
        ~CreateInteractionChoiceSet();

        void setInteractionChoiceSetID(unsigned int s);
        void setChoiceSet(const std::vector<Choice>& s);

        unsigned int getInteractionChoiceSetID() const;
        const std::vector<Choice>& getChoiceSet() const;

    private:
        unsigned int interactionChoiceSetID;
        std::vector<Choice> choiceSet;
        friend class CreateInteractionChoiceSetMarshaller;
    };
}

#endif