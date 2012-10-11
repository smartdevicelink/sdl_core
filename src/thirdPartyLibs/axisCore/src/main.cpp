#include <iostream>
#include <IProtocolObserver.hpp>
#include <ProtocolHandler.hpp>

//#include "MessageGenerator/CMessage.hpp"
//#include "MessageGenerator/CMessage.cpp"

//CMessage msg;

using namespace std;
using namespace AxisCore;

class ProtocolObserver : public IProtocolObserver
{
public:
    ProtocolHandler *mHandler;
    UInt8 mSessionID;


    ProtocolObserver()
    {
        cout << "enter ProtocolObserver() \n";
        mSessionID = 0;

        CMessage::generateInitialMessage();
        mHandler = new ProtocolHandler(this, 0);
        mHandler->dataReceived();        
        //CMessage::generateSingleMessage("Hello ?");
        CMessage::generateMultipleMessages("Hello ?", 5);
        for (int i = 0 ; i < 5 ; i++)
            mHandler->dataReceived();

        //CMessage::generateFinalMessage();
        //mHandler->dataReceived();
    }

    ~ProtocolObserver()
    {
        delete mHandler;
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
        cout << "dataReceivedCallback size : " << dataSize << "\n";

        UInt8 *data = new UInt8[dataSize];

        mHandler->receiveData(sessionID, messageID, SERVICE_TYPE_RPC, dataSize, data);

        std::cout << "********  RESULT :  " << std::string((char*)data, dataSize) <<  " size : " << dataSize << std:: endl;

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

