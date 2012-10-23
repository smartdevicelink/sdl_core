#ifndef TTS_SPEAK_CLASS
#define TTS_SPEAK_CLASS

#include "JSONHandler/RPC2Request.h"
#include "JSONHandler/TTSChunk.h"
#include <vector>

namespace RPC2Communication
{
    class Speak : public RPC2Request
    {
    public:
        Speak();
        virtual ~Speak();

        virtual const std::vector<TTSChunk> & getTTSChunks() const;
        virtual void setTTSChunks( const std::vector<TTSChunk>& s );

    private:
        std::vector<TTSChunk> mTTSChunks;

        friend class SpeakMarshaller;
    };
}

#endif