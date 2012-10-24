#ifndef SETGLOBAL_PROPERTIES_CLASS
#define SETGLOBAL_PROPERTIES_CLASS

#include "JSONHandler/RPC2Request.h"
#include "JSONHandler/TTSChunk.h"
#include <string>

namespace RPC2Communication 
{
    class SetGlobalProperties : public RPC2Request
    {
    public:
        SetGlobalProperties();
        ~SetGlobalProperties();
    
        void setHelpPrompt( const TTSChunk& helpPrompt );
        void setTimeoutPrompt( const TTSChunk& timeoutPrompt );

        TTSChunk getHelpPrompt() const;
        TTSChunk getTimeoutPrompt() const;

    private:
        TTSChunk mHelpPrompt;
        TTSChunk mTimeoutPrompt;
        friend class SetGlobalPropertiesMarshaller;
    };
}

#endif