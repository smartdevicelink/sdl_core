#include "JSONHandler/SetGlobalProperties.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

SetGlobalProperties::SetGlobalProperties()
:RPC2Request( RPC2Marshaller::METHOD_SET_GLOBAL_PROPERTIES_REQUEST )
{
    mHelpPrompt = 0;
    mTimeoutPrompt = 0;
}
 
SetGlobalProperties::~SetGlobalProperties()
{
    if (mHelpPrompt)
    {
        delete mHelpPrompt;
        mHelpPrompt = 0;
    }
    if (mTimeoutPrompt)
    {
        delete mTimeoutPrompt;
        mTimeoutPrompt = 0;
    }        
}
    
void SetGlobalProperties::setHelpPrompt( const std::vector<TTSChunk>& helpPrompt )
{
    delete mHelpPrompt;
    mHelpPrompt = new std::vector<TTSChunk>(helpPrompt);
}

void SetGlobalProperties::setTimeoutPrompt( const std::vector<TTSChunk>& timeoutPrompt )
{
    delete mTimeoutPrompt;
    mTimeoutPrompt = new std::vector<TTSChunk>(timeoutPrompt);
}

const std::vector<TTSChunk>* SetGlobalProperties::getHelpPrompt() const
{
    return mHelpPrompt;
}

const std::vector<TTSChunk>* SetGlobalProperties::getTimeoutPrompt() const
{
    return mTimeoutPrompt;
}