#include "JSONHandler/DeleteInteractionChoiceSet.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

DeleteInteractionChoiceSet::DeleteInteractionChoiceSet()
:RPC2Request( RPC2Marshaller::METHOD_DELETEINTERACTIONCHOICESET_REQUEST )
{}
 
DeleteInteractionChoiceSet::~DeleteInteractionChoiceSet()
{}

void DeleteInteractionChoiceSet::setInteractionChoiceSetId(unsigned int s)
{
    interactionChoiceSetID = s;
}
        
unsigned int DeleteInteractionChoiceSet::getInteractionChoiceSetId() const
{
    return interactionChoiceSetID;
}