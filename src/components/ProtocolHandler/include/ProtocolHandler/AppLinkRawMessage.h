#ifndef APPLINKRAWMESSAGE_CLASS
#define APPLINKRAWMESSAGE_CLASS

namespace NsProtocolHandler
{
    class AppLinkRawMessage
    {
    public:
        AppLinkRawMessage(int connectionKey,
                            unsigned int protocolVersion,
                            unsigned char * data,
                            unsigned int dataSize);
        ~AppLinkRawMessage();

        int getConnectionKey() const;
        unsigned char * getData() const;
        unsigned int getDataSize() const;
        unsigned int getProtocolVersion() const;
    
    private:
        int mConnectionKey;
        unsigned char * mData;
        unsigned int mDataSize;
        unsigned int mProtocolVersion;
    };
}

#endif //  APPLINKRAWMESSAGE_CLASS