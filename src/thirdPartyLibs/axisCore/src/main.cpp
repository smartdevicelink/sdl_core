#include <iostream>
#include <memory.h>
#include <stdio.h>
#include <sstream>
#include <IProtocolObserver.hpp>
#include <ProtocolHandler.hpp>

//#include "MessageGenerator/CMessage.hpp"
//#include "MessageGenerator/CMessage.cpp"
#include "../../transport/bt/BluetoothAPI.hpp"

using namespace std;
using namespace AxisCore;
using namespace Bluetooth;

class ProtocolObserver : public IProtocolObserver
{
public:
    ProtocolHandler *mHandler;

    ProtocolObserver()
    {
        cout << "enter ProtocolObserver() \n";

        CMessage *msgGen = new CMessage();
        mHandler = new ProtocolHandler(this, msgGen, 2);
        msgGen->initBluetooth(mHandler);

        msgGen->generateInitialMessage(0x07);

        msgGen->generateInitialMessage(0x0F);

        std::string str;

        for (int i = 0 ; i < 50 ; i++)
        {
            std::stringstream stream;
            stream << i;
            str.append(stream.str() );
            str.append("_");
        }

        msgGen->generateMultipleMessages(2, 0x07, 1, str, 5);
        //msgGen->generateSingleMessage(2, 0x07, 1, str);

        delete msgGen;
    }

    ~ProtocolObserver()
    {
        delete mHandler;
    }

    virtual void sessionStartedCallback(const UInt8 sessionID, const UInt32 hashCode)
    {
        cout << "sessionStartedCallback : Session ID : 0x" << std::hex << (int)sessionID << "\n";
    }

    virtual void sessionEndedCallback(const UInt8 sessionID)
    {
        cout << "sessionEndedCallback : Session ID : 0x" << std::hex << (int)sessionID << "\n";
    }

    virtual void dataReceivedCallback(const UInt8 sessionID, const UInt32 messageID, const UInt32 dataSize)
    {
        cout << "dataReceivedCallback size : " << std::dec << dataSize << "\n";

        UInt8 *data = new UInt8[dataSize];

        mHandler->receiveData(sessionID, messageID, SERVICE_TYPE_RPC, dataSize, data);

        std::cout << "RECEIVED DATA, sessionID: " << std::hex << (int)sessionID << std::endl;
        std::cout << "RECEIVED DATA PAYLOAD: " << std::string((char*)data, dataSize) <<  "\n SIZE : "
                  << std::dec << dataSize << std::endl;

        mHandler->sendData(1, 0x07, dataSize, data, false);

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

