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
ProtocolPacketHeader CMessage::sentHeader = ProtocolPacketHeader();
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

void CMessage::saveSentHeader()
{
   sentHeader.version = sVersion;
   sentHeader.compress = sCompressedFlag;
   sentHeader.frameType = sFrameType;
   sentHeader.serviceType = sServiceType;
   sentHeader.frameData = sFrameData;
   sentHeader.sessionID = sSessionID;
   sentHeader.dataSize = sDataSize;
}

void CMessage::generateInitialMessage(UInt8 serviceType, UInt8 sessionID)
{
   sVersion        = 0x01;
   sCompressedFlag = 0;
   sFrameType      = 0x00; //Control frame
   sServiceType    = serviceType;
   sFrameData      = 0x01; //Start session
   sSessionID      = sessionID;
   sDataSize       = 0x00;
   //sMessageID      = rand() % 0xFFFFFFFF + 1;

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
   //memcpy(sPacketData + 8, &sMessageID, 4);

   blobQueue.push(Blob((UInt8*)sPacketData, 8, blobQueue.size()));
}

void CMessage::generateSingleMessage(UInt8 serviceType, UInt8 sessionID, std::string payload)
{
   sVersion        = 0x01;
   sCompressedFlag = 0;
   sFrameType      = 0x01; //Single
   sServiceType    = serviceType;
   sFrameData      = 0x00; //Single Frame
   sSessionID      = sessionID;
   sDataSize       = payload.length() + 1; //
   //sMessageID      = rand() % 0xFFFFFFFF + 1;

   dispayField();

   sPacketData = malloc(8 + sDataSize);

   UInt8 firstByte = ( (sVersion << 4) & 0xF0 )
                   | ( (sCompressedFlag << 3) & 0x08)
                   | (sFrameType & 0x07);

   memcpy(sPacketData, &firstByte, 1);
   memcpy(sPacketData + 1, &sServiceType, 1);
   memcpy(sPacketData + 2, &sFrameData, 1);
   memcpy(sPacketData + 3, &sSessionID, 1);
   memcpy(sPacketData + 4, &sDataSize, 4);
   //memcpy(sPacketData + 8, &sMessageID, 4);
   memcpy(sPacketData + 8, (void*)const_cast<char*>(payload.c_str()), sDataSize);

   std::cout << "********    " << std::string((char*)sPacketData, sDataSize + 12) <<  std:: endl;

   blobQueue.push(Blob((UInt8*)sPacketData, 8 + sDataSize, blobQueue.size()));
}

void CMessage::generateFinalMessage(UInt8 serviceType, UInt8 sessionID)
{
   sVersion        = 0x01;
   sCompressedFlag = 0;
   sFrameType      = 0x00; //Control frame
   sServiceType    = serviceType;
   sFrameData      = 0x04; //Start session
   sSessionID      = sessionID;
   sDataSize       = 0x00;
  //sMessageID      = rand() % 0xFFFFFFFF + 1;

   dispayField();

   sPacketData = malloc(8);

   UInt8 firstByte = ( (sVersion << 4) & 0xF0 )
                   | ( (sCompressedFlag << 3) & 0x08)
                   | (sFrameType & 0x07);

  memcpy(sPacketData, &firstByte, 1);
  memcpy(sPacketData + 1, &sServiceType, 1);
  memcpy(sPacketData + 2, &sFrameData, 1);
  memcpy(sPacketData + 3, &sSessionID, 1);
  memcpy(sPacketData + 4, &sDataSize, 4);
  //memcpy(sPacketData + 8, &sMessageID, 4);

  blobQueue.push(Blob((UInt8*)sPacketData, 8, blobQueue.size()));
}

void CMessage::generateMultipleMessages(UInt8 serviceType, UInt8 sessionID, std::string payload, int messagesQuantity)
{
   //sMessageID      = rand() % 0xFFFFFFFF + 1;

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

      sServiceType    = serviceType;

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

      sSessionID      = sessionID;

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

      sPacketData = malloc(8 + sDataSize);

      UInt8 firstByte = ( (sVersion << 4) & 0xF0 )
                        | ( (sCompressedFlag << 3) & 0x08)
                        | (sFrameType & 0x07);

      memcpy(sPacketData, &firstByte, 1);
      memcpy(sPacketData + 1, &sServiceType, 1);
      memcpy(sPacketData + 2, &sFrameData, 1);
      memcpy(sPacketData + 3, &sSessionID, 1);
      memcpy(sPacketData + 4, &sDataSize, 4);
      //memcpy(sPacketData + 8, &sMessageID, 4);

      if(0 != i)
      {
         memcpy(sPacketData + 8, (void*)const_cast<char*>(payload.c_str()), sDataSize);
      }
      else
      {
         memcpy(sPacketData + 8, &totalConsecutivePayloadSize, 4);
         memcpy(sPacketData + 12, &numberOfConsecutiveFrames, 4);
      }


      blobQueue.push(Blob((UInt8*)sPacketData, 8 + sDataSize, blobQueue.size()));
   }
}

UInt32 CMessage::verify(ProtocolPacketHeader& header, UInt32 fieldsToValidate)
{
   UInt32 returnValue;

   //Get zero bit. Corresponds to version field
   if(getBit(fieldsToValidate, 0))
   {
      if(header.version == sentHeader.version)
      {
         clearBit(returnValue, 0);
      }
      else
      {
         setBit(returnValue, 0);
      }
   }
   else
   {
      clearBit(returnValue, 0);
   }

   //Get first bit. Corresponds to compress field
   if(getBit(fieldsToValidate, 1))
   {
      if(header.compress == sentHeader.compress)
      {
         clearBit(returnValue, 1);
      }
      else
      {
         setBit(returnValue, 1);
      }
   }
   else
   {
      clearBit(returnValue, 1);
   }

   //Get second bit. Corresponds to frameType field
   if(getBit(fieldsToValidate, 2))
   {
      if(header.frameType == sentHeader.frameType)
      {
         clearBit(returnValue, 2);
      }
      else
      {
         setBit(returnValue, 2);
      }
   }
   else
   {
      clearBit(returnValue, 2);
   }

   //Get third bit. Corresponds to serviceType field
   if(getBit(fieldsToValidate, 3))
   {
      if(header.serviceType == sentHeader.serviceType)
      {
         clearBit(returnValue, 3);
      }
      else
      {
         setBit(returnValue, 3);
      }
   }
   else
   {
      clearBit(returnValue, 3);
   }

   //Get fourth bit. Corresponds to frameData field
   if(getBit(fieldsToValidate, 4))
   {
      if(header.frameData == sentHeader.frameData)
      {
         clearBit(returnValue, 4);
      }
      else
      {
         setBit(returnValue, 4);
      }
   }
   else
   {
      clearBit(returnValue, 4);
   }

   //Get fifth bit. Corresponds to sessionID field
   if(getBit(fieldsToValidate, 5))
   {
      if(header.sessionID == sentHeader.sessionID)
      {
         clearBit(returnValue, 5);
      }
      else
      {
         setBit(returnValue, 5);
      }
   }
   else
   {
      clearBit(returnValue, 5);
   }

   //Get sixth bit. Corresponds to dataSize field
   if(getBit(fieldsToValidate, 6))
   {
      if(header.dataSize == sentHeader.dataSize)
      {
         clearBit(returnValue, 6);
      }
      else
      {
         setBit(returnValue, 6);
      }
   }
   else
   {
      clearBit(returnValue, 6);
   }

   return returnValue;
}

int CMessage::getBit(const UInt32 value, const UInt32 position)
{
   return (value & ( 1 << position )) >> position;
}

void CMessage::setBit(UInt32 value, const UInt32 position)
{
   value |= 1 << position;
}

void CMessage::clearBit(UInt32 value, const UInt32 position)
{
   value &= ~(1 << position);
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

