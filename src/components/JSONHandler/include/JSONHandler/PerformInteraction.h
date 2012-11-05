#ifndef PERFORMINTERACTION
#define PERFORMINTERACTION

#include "JSONHandler/RPC2Request.h"
#include "JSONHandler/ALRPCObjects/TTSChunk.h"
#include "JSONHandler/ALRPCObjects/InteractionMode.h"
#include <vector>

namespace RPC2Communication
{
    class PerformInteraction : public RPC2Request
    {
    public:
        PerformInteraction();
        ~PerformInteraction();

        void setInitialText(const std::string & initialText);
        void setInitialPrompt(const std::vector<TTSChunk> & initialPrompt);
        void setInteractionMode(const InteractionMode& interactionMode);
        void setInteractionChoiceSetIDList(const std::vector<unsigned int> & interactionChoiceSetIDList);
        void setHelpPrompt(const std::vector<TTSChunk>& helpPrompt);
        void setTimeoutPrompt(const std::vector<TTSChunk>& timeoutPrompt);
        void setTimeout(unsigned int timeout);

        const std::string& getInitialText() const;
        const std::vector<TTSChunk> & getInitialPrompt() const;
        const InteractionMode& getInteractionMode() const;
        const std::vector<unsigned int>& getInteractionChoiceSetIDList() const;
        std::vector<TTSChunk> * getHelpPrompt() const;
        std::vector<TTSChunk> * getTimeoutPrompt() const;
        unsigned int * getTimeout() const;
    
    private:
        std::string initialText;
        std::vector<TTSChunk> initialPrompt;
        InteractionMode interactionMode;
        std::vector<unsigned int> interactionChoiceSetIDList;
        std::vector<TTSChunk> * helpPrompt;
        std::vector<TTSChunk> * timeoutPrompt;
        unsigned int * timeout;
        friend class PerformInteractionMarshaller;
    };
}

#endif