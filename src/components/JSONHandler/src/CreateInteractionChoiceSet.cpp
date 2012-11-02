#include "JSONHandler/CreateInteractionChoiceSet.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

 CreateInteractionChoiceSet::CreateInteractionChoiceSet()
 :RPC2Request( RPC2Marshaller :: METHOD_CREATEINTERACTIONCHOICESET_REQUEST )
 {}
  
 CreateInteractionChoiceSet::~CreateInteractionChoiceSet()
 {}

void  CreateInteractionChoiceSet::setInteractionChoiceSetID(unsigned int s)
{
    interactionChoiceSetID = s;
}
    
void  CreateInteractionChoiceSet::setChoiceSet(const std::vector<Choice>& s)
{
    choiceSet = s;
}

unsigned int  CreateInteractionChoiceSet::getInteractionChoiceSetID() const
{
    return interactionChoiceSetID;
}
    
const std::vector<Choice>&  CreateInteractionChoiceSet::getChoiceSet() const
{
    return choiceSet;
}