#ifndef APPLINKRAWMESSAGE_CLASS
#define APPLINKRAWMESSAGE_CLASS

namespace NsProtocolHandler
{
    class AppLinkRawMessage
    {
    public:
        AppLinkRawMessage(unsigned int connectionID,
                            unsigned int sessionID,
                            unsigned int protocolVersion,
                            unsigned int * data,
                            unsigned int dataSize);
        ~AppLinkRawMessage();

        unsigned int getConnectionID() const;
        unsigned int getSessionID() const;
        unsigned int * getData() const;
        unsigned int getDataSize() const;
        unsigned int getProtocolVersion() const;
    
    private:
        unsigned int mConnectionID;
        unsigned int mSessionID;
        unsigned int * mData;
        unsigned int mDataSize;
        unsigned int mProtocolVersion;
        //TODO - check if this is needed.
        //unsigned int hashCode;
    };
}

#endif //  APPLINKRAWMESSAGE_CLASS