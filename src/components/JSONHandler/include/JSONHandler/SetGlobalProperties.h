#ifndef SETGLOBAL_PROPERTIES_CLASS
#define SETGLOBAL_PROPERTIES_CLASS

#include "JSONHandler/RPC2Request.h"
#include "JSONHandler/ALRPCObjects/TTSChunk.h"
#include <string>
#include <vector>

namespace RPC2Communication 
{
    class SetGlobalProperties : public RPC2Request
    {
    public:
        SetGlobalProperties();
        ~SetGlobalProperties();
    
        void setHelpPrompt( const std::vector<TTSChunk>& helpPrompt );
        void setTimeoutPrompt( const std::vector<TTSChunk>& timeoutPrompt );

        const std::vector<TTSChunk>* getHelpPrompt() const;
        const std::vector<TTSChunk>* getTimeoutPrompt() const;

    private:
        std::vector<TTSChunk>* mHelpPrompt;
        std::vector<TTSChunk>* mTimeoutPrompt;
        friend class SetGlobalPropertiesMarshaller;
    };
}

#endif