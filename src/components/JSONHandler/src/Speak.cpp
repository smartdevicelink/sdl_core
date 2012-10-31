#include "JSONHandler/Speak.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

Speak::Speak()
:RPC2Request( RPC2Marshaller::METHOD_SPEAK_REQUEST )
{}
 
Speak::~Speak()
{}

const std::vector<TTSChunk> & Speak::getTTSChunks() const
{
    return mTTSChunks;
}
 
void Speak::setTTSChunks( const std::vector<TTSChunk>& s )
{
    mTTSChunks = s;
}