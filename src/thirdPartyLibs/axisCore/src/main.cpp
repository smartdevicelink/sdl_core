#include <iostream>
#include <IProtocolObserver.hpp>
#include <ProtocolHandler.hpp>

using namespace std;



class ProtocolObserver : public IProtocolObserver
{
public:
    ProtocolHandler *mHandler;
    UInt8 mSessionID;


    ProtocolObserver()
    {
        cout << "enter ProtocolObserver() \n";
        mSessionID = 0;
        mHandler = new ProtocolHandler(this);
    }

    virtual void sessionStartedCallback(const UInt8 sessionID)
    {
        mSessionID = sessionID;
        cout << "sessionStartedCallback\n Session ID : " << sessionID;
    }

    virtual void sessionEndedCallback(const UInt8 sessionID)
    {
        cout << "sessionEndedCallback\n";
    }

    virtual void dataReceivedCallback(const UInt8 sessionID, const UInt32 messageID, const UInt32 dataSize)
    {
        cout << "dataReceivedCallback\n data : \n";

        UInt8 *data = new UInt8[dataSize];

        mHandler->ReceiveData(sessionID, messageID, SERVICE_TYPE_RPC, dataSize, data);
        cout << data;

        delete [] data;
    }
};



int main()
{
    cout << "enter main() \n";

    ProtocolObserver *observer = new ProtocolObserver();
    //ProtocolHandler hand(observer);
    //hand.startSession(0x07);
    //hand.endSession(0);



    delete observer;

    cout << "exit main() \n";
    return 0;
}

