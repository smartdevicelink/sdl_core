#ifndef IPROTOCOLOBSERVER_CLASS
#define IPROTOCOLOBSERVER_CLASS 

namespace NsProtocolHandler
{
    class IProtocolObserver
    {
    public:    
        virtual void onDataReceivedCallback(unsigned int sessionID, unsigned int dataSize, unsigned int * data) = 0;

    protected:
        virtual ~IProtocolObserver() {}

    };
}

#endif //IPROTOCOLOBSERVER_CLASS