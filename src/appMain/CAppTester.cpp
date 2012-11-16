/**
 * \file CBTAdapter.cpp
 * \brief Bluetooth adapter for AppLink.
 * \author AKara
 */

#include "CBTAdapter.hpp"
#include "btinclude.h"
#include <stdlib.h>
#include "CAppTester.hpp"
#include <fstream>

namespace NsApplicationTester
{

   CAppTester::CAppTester():
   mpProtocolHandler(NULL)
   {
   }

   CAppTester::~CAppTester()
   {
   }

   int CAppTester::startSession(int count)
   {
      printf("%s:%d CAppTester::startSession()\n", __FILE__, __LINE__);

      printf("Start session (RPC Service)\n");
      void* sPacketData = malloc(8);
      memset(sPacketData, 0, 8);
      ((UInt8*)sPacketData)[0] = 16;
      ((UInt8*)sPacketData)[1] = 7;
      ((UInt8*)sPacketData)[2] = 1;
      blobQueue.push(Blob((UInt8*)sPacketData, 8, blobQueue.size()));
      if (NULL != mpProtocolHandler)
      {
         mpProtocolHandler->dataReceived();
      }
      if (1 < count)
      {
         printf("Start session (Bulk Service)\n");
         sPacketData = malloc(8);
         memset(sPacketData, 0, 8);
         ((UInt8*)sPacketData)[0] = 16;
         ((UInt8*)sPacketData)[1] = 15;
         ((UInt8*)sPacketData)[2] = 1;
         blobQueue.push(Blob((UInt8*)sPacketData, 8, blobQueue.size()));
         if (NULL != mpProtocolHandler)
         {
            mpProtocolHandler->dataReceived();
         }
      }
      return count;
   }
   
   int CAppTester::sendDataFromFile(const char * fileName)
   {
      using namespace std;
      printf("%s:%d CAppTester::sendDataFromFile()\n", __FILE__, __LINE__);

      ifstream file_str;
      file_str.open (fileName);

      if (!file_str.is_open())
      {
        printf("File wasn't opened!\n");
        return -1;
      }

      int startpos = 0;
      file_str.seekg(0, ios::end);
      int length = file_str.tellg();
      file_str.seekg(0, ios::beg);
      printf("length = %d \n", length);
      while (startpos < length)
      {
        char * raw_data = new char[length];
        file_str.getline(raw_data, length);
        printf("strlen = %d \n", strlen(raw_data));
        startpos += strlen(raw_data)+1;
        std::string instr = std::string(raw_data, strlen(raw_data));
        delete[] raw_data;
        printf("Input string:\n %s\n", instr.c_str());
        // create a Blob and send
        generateSingleMessage(1, 7, 1, instr);
        printf("length = %d \n", length);
        printf("startpos = %d \n", startpos);
      }
      file_str.close();
      return 0;
   }

  void CAppTester::generateSingleMessage(UInt8 protocolVersion,
                                       UInt8 serviceType,
                                       UInt8 sessionID,
                                       std::string payload)
  {
     UInt32 headerSize = 8;
     if (protocolVersion == 0x02)
        headerSize = 12;

     UInt8 sVersion        = protocolVersion;
     UInt8 sCompressedFlag = 0;
     UInt8 sFrameType      = 0x01; //Single
     UInt8 sServiceType    = serviceType;
     UInt8 sFrameData      = 0x00; //Single Frame
     UInt8 sSessionID      = sessionID;
     UInt32 sDataSize       = payload.length();//' + 1; //
     UInt32 sMessageID      = 12345;

     void* sPacketData = malloc(headerSize + sDataSize);

     UInt8 firstByte = ( (sVersion << 4) & 0xF0 )
                     | ( (sCompressedFlag << 3) & 0x08)
                     | (sFrameType & 0x07);

     UInt32 offset = 0;
     memcpy((UInt8*)sPacketData + offset++, &firstByte, 1);
     memcpy((UInt8*)sPacketData + offset++, &sServiceType, 1);
     memcpy((UInt8*)sPacketData + offset++, &sFrameData, 1);
     memcpy((UInt8*)sPacketData + offset++, &sSessionID, 1);

     UInt8 tmp = sDataSize >> 24;
     memcpy((UInt8*)sPacketData + offset++, &tmp, 1);
     tmp = sDataSize >> 16;
     memcpy((UInt8*)sPacketData + offset++, &tmp, 1);
     tmp = sDataSize >> 8;
     memcpy((UInt8*)sPacketData + offset++, &tmp, 1);
     tmp = sDataSize;
     memcpy((UInt8*)sPacketData + offset++, &tmp, 1);


     if (protocolVersion == 0x02)
     {
         UInt8 tmp1 = sMessageID >> 24;
         memcpy((UInt8*)sPacketData + offset++, &tmp1, 1);
         tmp1 = sMessageID >> 16;
         memcpy((UInt8*)sPacketData + offset++, &tmp1, 1);
         tmp1 = sMessageID >> 8;
         memcpy((UInt8*)sPacketData + offset++, &tmp1, 1);
         tmp1 = sMessageID;
         memcpy((UInt8*)sPacketData + offset++, &tmp1, 1);
     }

     memcpy((UInt8*)sPacketData + offset, (void*)const_cast<char*>(payload.c_str()), sDataSize);

     printf("SINGLE MESSAGE GENERATED!\n");

     blobQueue.push(Blob((UInt8*)sPacketData, offset + sDataSize, blobQueue.size()));

     mpProtocolHandler->dataReceived();
  }

  void CAppTester::initBluetooth(Bluetooth::IBluetoothHandler * pHandler)
  {
      printf("%s:%d CAppTester::initBluetooth()\n", __FILE__, __LINE__);
      if (NULL != pHandler)
      {
         mpProtocolHandler = pHandler;
      }
  }

  void CAppTester::deinitBluetooth()
  {
      printf("%s:%d CAppTester::deinitBluetooth()\n", __FILE__, __LINE__);
  }

  const Blob CAppTester::getBuffer()
  {
      printf("%s:%d CBTAdapter::getBuffer()\n", __FILE__, __LINE__);
      return blobQueue.front();
  }

  void CAppTester::releaseBuffer(const Blob& blob)
  {
      printf("%s:%d CAppTester::releaseBuffer()\n", __FILE__, __LINE__);
      free(blob.buffer());
      blobQueue.pop();
  }

  void CAppTester::sendBuffer(UInt8 * pBuffer, size_t size)
  {
      printf("%s:%d CAppTester::sendBuffer()\n", __FILE__, __LINE__);
      std::string str = std::string((const char*)pBuffer, size);
      printf("%s\n", str.c_str());
  }
} /* namespace NsApplicationTester */
