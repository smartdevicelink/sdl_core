#include <iostream>
#include <memory.h>
#include <stdio.h>
#include <IProtocolObserver.hpp>
#include <ProtocolHandler.hpp>

//#include "MessageGenerator/CMessage.hpp"
//#include "MessageGenerator/CMessage.cpp"

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

        CMessage::generateInitialMessage(0x07, 0);
        mHandler = new ProtocolHandler(this, 0);
        mHandler->dataReceived();
        CMessage::generateInitialMessage(0x07, 0);
        mHandler->dataReceived();
        std::string str;
        for (int i = 0 ; i < 278 ; i++)
            str.append("1");
        CMessage::generateSingleMessage(0x07, 0, str);
        //CMessage::generateMultipleMessages("Hello ?", 5);
        //for (int i = 0 ; i < 5 ; i++)
            mHandler->dataReceived();
            mHandler->sendData(0, 7, 278, (UInt8*)const_cast<char*>(str.c_str()), false);

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

    delete observer;
    cout << "exit main() \n";
    return 0;
}

