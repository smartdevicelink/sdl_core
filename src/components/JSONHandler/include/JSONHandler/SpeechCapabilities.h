#ifndef SPEECH_CAPABILITIES_CLASS
#define SPEECH_CAPABILITIES_CLASS

class SpeechCapabilities
{
public:
    enum SpeechCapabilitiesInternal
    {
        INVALID_ENUM = -1,
        TEXT = 0,
        SAPI_PHONEMES = 1,
        PRE_RECORDED = 2,
        SILENCE = 3
    };

    SpeechCapabilities() : mInternal( INVALID_ENUM ) {}
    SpeechCapabilities(SpeechCapabilitiesInternal e) : mInternal(e) {}
    ~SpeechCapabilities() {}

    SpeechCapabilitiesInternal get(void) const { return mInternal; }
    void set(SpeechCapabilitiesInternal e) { mInternal = e; }

private:
    SpeechCapabilitiesInternal mInternal;
    friend class SpeechCapabilitiesMarshaller;
};

#endif