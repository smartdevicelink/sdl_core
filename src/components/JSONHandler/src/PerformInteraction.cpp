#include "JSONHandler/PerformInteraction.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

PerformInteraction::PerformInteraction()
:RPC2Request( RPC2Marshaller::METHOD_PERFORMINTERACTION_REQUEST)
,helpPrompt(0)
,timeoutPrompt(0)
,timeout(0)
{
    std::string initialText;
        std::vector<TTSChunk> initialPrompt;
        InteractionMode interactionMode;
        std::vector<unsigned int> interactionChoiceSetIDList;
        std::vector<TTSChunk> * helpPrompt;
        std::vector<TTSChunk> * timeoutPrompt;
        unsigned int * timeout;
}
 
PerformInteraction::~PerformInteraction()
{
    if ( helpPrompt )
        delete helpPrompt;
    if ( timeoutPrompt )
        delete timeoutPrompt;
    if ( timeout )
        delete timeout;
}

void PerformInteraction::setInitialText(const std::string & initialText)
{
    this->initialText = initialText;
}
        
void PerformInteraction::setInitialPrompt(const std::vector<TTSChunk> & initialPrompt)
{
    this->initialPrompt = initialPrompt;
}

void PerformInteraction::setInteractionMode(const InteractionMode& interactionMode)
{
    this->interactionMode = interactionMode;
}
        
void PerformInteraction::setInteractionChoiceSetIDList(const std::vector<unsigned int> & interactionChoiceSetIDList)
{
    this->interactionChoiceSetIDList = interactionChoiceSetIDList;
}
        
void PerformInteraction::setHelpPrompt(const std::vector<TTSChunk>& helpPrompt)
{
    if (this->helpPrompt)
        delete this->helpPrompt;
    this->helpPrompt = new std::vector<TTSChunk> (helpPrompt);
}
        
void PerformInteraction::setTimeoutPrompt(const std::vector<TTSChunk>& timeoutPrompt)
{
    if (this->timeoutPrompt)
        delete this->timeoutPrompt;
    this->timeoutPrompt = new std::vector<TTSChunk> (timeoutPrompt);
}
        
void PerformInteraction::setTimeout(unsigned int timeout)
{
    if (this->timeout)
        delete this->timeout;
    this->timeout = new unsigned int (timeout);
}

const std::string& PerformInteraction::getInitialText() const
{
    return initialText;
}
        
const std::vector<TTSChunk> & PerformInteraction::getInitialPrompt() const
{
    return initialPrompt;
}
        
const InteractionMode& PerformInteraction::getInteractionMode() const
{
    return interactionMode;
}
        
const std::vector<unsigned int>& PerformInteraction::getInteractionChoiceSetIDList() const
{
    return interactionChoiceSetIDList;
}

std::vector<TTSChunk> * PerformInteraction::getHelpPrompt() const
{
    return helpPrompt;
}
        
std::vector<TTSChunk> * PerformInteraction::getTimeoutPrompt() const
{
    return timeoutPrompt;
}

unsigned int * PerformInteraction::getTimeout() const
{
    return timeout;
}
