/**
 * \file CAppTester.hpp
 * \brief Application tester class.
 * \ Reads JSON strings from file and sends them to ProtocolHandler for test purposes.
 * \author AKara
 */

#ifndef APPTESTER_H 
#define APPTESTER_H 

#include <stdint.h>
#include <string>
#include <vector>
#include <queue>

//#include "Types.hpp"
//#include "Blob.hpp"

/**
 * \namespace NsApplicationTester
 * \brief AppLink tester for high level.
 */ 
namespace NsApplicationTester
{
   /**
    * \class CBTAdapter
    * \brief Bluetooth adapter for AppLink
    */
   class CAppTester/*: public Bluetooth::IBluetoothAPI*/
   {
      public:
        /**
        * \brief Constructor.
        */
        CAppTester();

        /**
        * \brief Destructor.
        */
        ~CAppTester();

        /**
        * \brief Starts session.
        * \param count count of sessions to start
        * \return Error code (-1 in case of any errors)
        */
		    int startSession(int count);

        /**
        * \brief Sends data from file to appLinkCore instead of device
        * \param fileName name of file with json messages
        * \return Error code (-1 in case of any errors)
        */
		    int sendDataFromFile(const char * fileName);

        /**
        * \brief inits BTAdapter. Gives pointer to ProtocolHandler.
        * \param pHandler pointer to ProtocolHandler.
        */
        //void initBluetooth(Bluetooth::IBluetoothHandler * pHandler);
 
        /**
        * \brief deinits BTAdapter.
        */
         void deinitBluetooth();

        /**
        * \brief gets Blob from BTAdapter que.
        * \return Blob with data from the que.
        */
        const Blob getBuffer();
 
        /**
        * \brief releases data from Blob and deletes it from que.
        * \param Blob with data from the que reference.
        */
        void releaseBuffer(const Blob& blob);

        /**
        * \brief Sends data to opened socket.
        * \param pBuffer pointer to the buffer with data to send.
        * \param size size of data frame.
        */
        void sendBuffer(UInt8 * pBuffer, size_t size);
      private:
        void generateSingleMessage(UInt8 protocolVersion,
                                       UInt8 serviceType,
                                       UInt8 sessionID,
                                       std::string payload);

        //Bluetooth::IBluetoothHandler * mpProtocolHandler;

        std::queue<Blob> blobQueue;
   };
}/* namespace NsApplicationTester */
#endif /* APPTESTER_H */