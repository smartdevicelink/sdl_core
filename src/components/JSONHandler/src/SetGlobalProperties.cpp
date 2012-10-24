#include "JSONHandler/SetGlobalProperties.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

SetGlobalProperties::SetGlobalProperties()
:RPC2Request( RPC2Marshaller::METHOD_SET_GLOBAL_PROPERTIES_REQUEST )
{}
 
SetGlobalProperties::~SetGlobalProperties()
{}
    
void SetGlobalProperties::setHelpPrompt( const TTSChunk& helpPrompt )
{
    mHelpPrompt = helpPrompt;
}

void SetGlobalProperties::setTimeoutPrompt( const TTSChunk& timeoutPrompt )
{
    mTimeoutPrompt = timeoutPrompt;
}

TTSChunk SetGlobalProperties::getHelpPrompt() const
{
    return mHelpPrompt;
}

TTSChunk SetGlobalProperties::getTimeoutPrompt() const
{
    return mTimeoutPrompt;
}