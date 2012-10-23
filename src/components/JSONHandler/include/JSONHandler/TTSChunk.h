#ifndef TTSCHUNK_CLASS
#define TTSCHUNK_CLASS

#include "SpeechCapabilities.h"

class TTSChunk
{
public:

    TTSChunk() : mText() {}
    ~TTSChunk() {}

    std::string getText(void) const { return mText; }
    void setText(const std::string& s) { mText = s; }

    SpeechCapabilities getSpeechCapabilities() const { return mType; }
    void setSpeechCapabilities(const SpeechCapabilities& s) { mType = s; }

private:
    std::string mText;
    SpeechCapabilities mType; 

    friend class TTSChunkMarshaller;
};

#endif