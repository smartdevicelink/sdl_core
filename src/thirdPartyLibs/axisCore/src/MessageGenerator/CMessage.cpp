#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iomanip>

#include "CMessage.hpp"
#include "../../transport/bt/Blob.hpp"

namespace AxisCore
{

UInt8 CMessage::sVersion = 0;
UInt8 CMessage::sCompressedFlag = 0;
UInt8 CMessage::sFrameType = 0;
UInt8 CMessage::sServiceType = 0;
UInt8 CMessage::sFrameData = 0;
UInt8 CMessage::sSessionID = 0;
UInt32 CMessage::sDataSize = 0;
UInt32 CMessage::sMessageID = 0;
void*  CMessage::sPacketData = 0;
std::queue<Blob> CMessage::blobQueue = std::queue<Blob>();

/*void CMessage::generate()
{
   sVersion        = 0x01; //rand() % (0x0F + 1); // 0x00 – 0x0F 4-Bits
   sCompressedFlag = 0; //rand() % (0x01 + 1); // 0x00 – 0x01 1-Bit
   //sFrameType    = rand() % (0x07 + 1); //0x00 – 0x07 3-Bit
   sFrameType      = 0x01; // SingleFrame

   //sServiceType    = rand() % (0xFF + 1); // 0x00 – 0xFF 8-Bits
   sServiceType    = 0x07; // 0x00 – 0xFF 8-Bits
   sFrameData      = rand() % (0xFF + 1); // 0x00 – 0xFF 8-Bits
   sSessionID      = 0; // 8-Bits //TODO: test multiple sessions
   sDataSize       = rand() % 0xFFFFFFFF + 1; //0x01-0xFFFFFFFF 32-Bits
   sMessageID      = rand() % 0xFFFFFFFF + 1; //0x01-0xFFFFFFFF 32-Bits

   dispayField();
}*/

void CMessage::dispayField()
{
   std::cout << std::hex << std::setw(4) << (int)sVersion
                         << std::setw(4) << (int)sCompressedFlag
                         << std::setw(4) << (int)sFrameType
                         << std::setw(4) << (int)sServiceType
                         << std::setw(4) << (int)sFrameData
                         << std::setw(4) << (int)sSessionID << std::endl;

   std::cout << std::hex /*<< std::setw(18)*/ << (int)sDataSize
                         << std::setw(18) << (int)sMessageID << std::endl;
}

void CMessage::generateInitialMessage()
{
   sVersion        = 0x01;
   sCompressedFlag = 0;
   sFrameType      = 0x00; //Control frame
   sServiceType    = 0x07;
   sFrameData      = 0x01; //Start session
   sSessionID      = 0;
   sDataSize       = 0x00;
   sMessageID      = rand() % 0xFFFFFFFF + 1;

   dispayField();

   sPacketData = malloc(12);

   UInt8 firstByte = ( (sVersion << 4) & 0xF0 )
                   | ( (sCompressedFlag << 3) & 0x08)
                   | (sFrameType & 0x07);

   memcpy(sPacketData, &firstByte, 1);
   memcpy(sPacketData + 1, &sServiceType, 1);
   memcpy(sPacketData + 2, &sFrameData, 1);
   memcpy(sPacketData + 3, &sSessionID, 1);
   memcpy(sPacketData + 4, &sDataSize, 4);
   memcpy(sPacketData + 8, &sMessageID, 4);

   blobQueue.push(Blob((UInt8*)sPacketData, 12, blobQueue.size()));
}

void CMessage::generateSingleMessage(std::string payload)
{
   sVersion        = 0x01;
   sCompressedFlag = 0;
   sFrameType      = 0x01; //Single
   sServiceType    = 0x07;
   sFrameData      = 0x00; //Single Frame
   sSessionID      = 0;
   sDataSize       = payload.length() + 1; //
   sMessageID      = rand() % 0xFFFFFFFF + 1;

   dispayField();

   sPacketData = malloc(12 + sDataSize);

   UInt8 firstByte = ( (sVersion << 4) & 0xF0 )
                   | ( (sCompressedFlag << 3) & 0x08)
                   | (sFrameType & 0x07);

   memcpy(sPacketData, &firstByte, 1);
   memcpy(sPacketData + 1, &sServiceType, 1);
   memcpy(sPacketData + 2, &sFrameData, 1);
   memcpy(sPacketData + 3, &sSessionID, 1);
   memcpy(sPacketData + 4, &sDataSize, 4);
   memcpy(sPacketData + 8, &sMessageID, 4);
   memcpy(sPacketData + 12, (void*)const_cast<char*>(payload.c_str()), sDataSize);

   std::cout << "********    " << std::string((char*)sPacketData, sDataSize + 12) <<  std:: endl;

   blobQueue.push(Blob((UInt8*)sPacketData, 12 + sDataSize, blobQueue.size()));
}

void CMessage::generateFinalMessage()
{
   sVersion        = 0x01;
   sCompressedFlag = 0;
   sFrameType      = 0x00; //Control frame
   sServiceType    = 0x07;
   sFrameData      = 0x04; //Start session
   sSessionID      = 0;
   sDataSize       = 0x00;
   sMessageID      = rand() % 0xFFFFFFFF + 1;

   dispayField();

   sPacketData = malloc(12);

   UInt8 firstByte = ( (sVersion << 4) & 0xF0 )
                   | ( (sCompressedFlag << 3) & 0x08)
                   | (sFrameType & 0x07);

  memcpy(sPacketData, &firstByte, 1);
  memcpy(sPacketData + 1, &sServiceType, 1);
  memcpy(sPacketData + 2, &sFrameData, 1);
  memcpy(sPacketData + 3, &sSessionID, 1);
  memcpy(sPacketData + 4, &sDataSize, 4);
  memcpy(sPacketData + 8, &sMessageID, 4);

  blobQueue.push(Blob((UInt8*)sPacketData, 12, blobQueue.size()));
}

void CMessage::generateMultipleMessages(std::string payload, int messagesQuantity)
{
   sMessageID      = rand() % 0xFFFFFFFF + 1;

   for(int i = 0; i < messagesQuantity; i++)
   {
      sVersion        = 0x01;
      sCompressedFlag = 0;

      if(0 == i)
      {
         sFrameType   = 0x02; //First frame
      }
      else
      {
         sFrameType   = 0x03; //Consecutive Frame
      }

      sServiceType    = 0x07;

      if(0 == i)
      {
         sFrameData   = 0x00; //First Frame
      }
      else if(0xFF == i)
      {
         sFrameData   = 0xFF;
      }
      else if (i == (messagesQuantity - 1) )
      {
         sFrameData = 0x00;
      }
      else
      {
         sFrameData = i % 0xFF;
      }

      sSessionID      = 0;

      UInt32 numberOfConsecutiveFrames;
      UInt32 totalConsecutivePayloadSize;
      if(0 == i)
      {
         numberOfConsecutiveFrames = messagesQuantity - 1;
         totalConsecutivePayloadSize = (payload.length() + 1) * numberOfConsecutiveFrames;

         sDataSize = 0x08;
      }
      else
      {
         sDataSize       = payload.length() + 1;
      }

      dispayField();

      sPacketData = malloc(12 + sDataSize);

      UInt8 firstByte = ( (sVersion << 4) & 0xF0 )
                        | ( (sCompressedFlag << 3) & 0x08)
                        | (sFrameType & 0x07);

      memcpy(sPacketData, &firstByte, 1);
      memcpy(sPacketData + 1, &sServiceType, 1);
      memcpy(sPacketData + 2, &sFrameData, 1);
      memcpy(sPacketData + 3, &sSessionID, 1);
      memcpy(sPacketData + 4, &sDataSize, 4);
      memcpy(sPacketData + 8, &sMessageID, 4);

      if(0 != i)
      {
         memcpy(sPacketData + 12, (void*)const_cast<char*>(payload.c_str()), sDataSize);
      }
      else
      {
         memcpy(sPacketData + 12, &totalConsecutivePayloadSize, sDataSize / 2);
         memcpy(sPacketData + 16, &numberOfConsecutiveFrames, sDataSize / 2);
      }


      blobQueue.push(Blob((UInt8*)sPacketData, 12 + sDataSize, blobQueue.size()));
   }
}

/*void CMessage::write()
{
   generate();

   sPacketData = malloc(12 + sDataSize);

   UInt8 firstByte = ( (sVersion << 4) & 0xF0 )
                       | ( (sCompressedFlag << 3) & 0x08)
                       | (sFrameType & 0x07);

   memcpy(sPacketData, &firstByte, 1);
   memcpy(sPacketData + 1, &sServiceType, 1);
   memcpy(sPacketData + 2, &sFrameData, 1);
   memcpy(sPacketData + 3, &sSessionID, 1);
   memcpy(sPacketData + 4, &sDataSize, 4);
   memcpy(sPacketData + 8, &sMessageID, 4);
}*/

Blob CMessage::getNextBlob()
{
   return blobQueue.front();
}

void CMessage::releaseCurrentBlob(const Blob& blob)
{
   free(blob.buffer());

   blobQueue.pop();
}

} //namespace AxisCore

