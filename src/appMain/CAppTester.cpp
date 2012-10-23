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
   }
   
   int CAppTester::sendDataFromFile(const char * fileName)
   {
      using namespace std;
      printf("%s:%d CAppTester::sendDataFromFile()\n", __FILE__, __LINE__);

      ifstream file_str;
      file_str.open ( fileName );

      file_str.seekg( 0, ios::end );
      int length = file_str.tellg();
      file_str.seekg( 0, ios::beg );

      char * raw_data = new char[length];
      file_str.read( raw_data, length );
      file_str.close();

/*}else if ((rfcommbuffer[0] == 17) && (rfcommbuffer[1] == 7))
{
   printf("Single frame RPC message!\n");
   unsigned int frameSize = (rfcommbuffer[4]<<24);
   frameSize |= (rfcommbuffer[5]<<16);
   frameSize |= (rfcommbuffer[6]<<8);
   frameSize |= rfcommbuffer[7];
   printf("frameSize = 0x%X\n", frameSize);
   unsigned char rfcommpayloadbuffer[1024];
   if (672 > frameSize)
   {// one L2CAP packet
      len = recv(sockid, rfcommpayloadbuffer, frameSize, 0);
   } else
   {
      //TODO: We need to read all L2CAP packets to get whole frame
   }
   if (len == frameSize)
   {
      void* sPacketData = malloc(8 + frameSize+1);
      memset(sPacketData, 0, 8 + frameSize+1);
      memcpy(sPacketData, rfcommbuffer, 8);
      memcpy((UInt8*)sPacketData + 8, rfcommpayloadbuffer, frameSize);
      blobQueue.push(Blob((UInt8*)sPacketData, frameSize+8, blobQueue.size()));

//                  std::string str = std::string((const char*)sPacketData+8, frameSize);
//                  printf("%s\n", str.c_str());

      if (NULL != mpProtocolHandler)
      {
         mpProtocolHandler->dataReceived();
      }
   } else
   {
      printf("Not All frame received frameSize = 0x%X\n", frameSize);
   }
*/
      return 0;
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
