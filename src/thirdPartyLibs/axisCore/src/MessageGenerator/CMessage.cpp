#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iomanip>

#include "CMessage.hpp"
#include "../../transport/bt/Blob.hpp"
#include "../../include/ProtocolPacketHeader.hpp"

namespace AxisCore
{

CMessage::CMessage()
 : sentHeader()
{}

void CMessage::dispayField()
{
   std::cout << "HEADER:" << std::endl;

   std::cout << std::hex << std::setw(4) << (int)sVersion
                         << std::setw(4) << (int)sCompressedFlag
                         << std::setw(4) << (int)sFrameType
                         << std::setw(4) << (int)sServiceType
                         << std::setw(4) << (int)sFrameData
                         << std::setw(4) << (int)sSessionID << std::endl;

   std::cout << "Data size : " << std::dec << (int)sDataSize << std::endl;
}

void CMessage::saveSentHeader()
{
   sentHeader->version = sVersion;
   sentHeader->compress = sCompressedFlag;
   sentHeader->frameType = sFrameType;
   sentHeader->serviceType = sServiceType;
   sentHeader->frameData = sFrameData;
   sentHeader->sessionID = sSessionID;
   sentHeader->dataSize = sDataSize;
}

void CMessage::generateInitialMessage(UInt8 serviceType)
{
   sVersion        = 0x01;
   sCompressedFlag = 0;
   sFrameType      = 0x00; //Control frame
   sServiceType    = serviceType;
   sFrameData      = 0x01; //Start session
   sSessionID      = 0;
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

   UInt8 tmp1 = sDataSize >> 24;
   memcpy(sPacketData + 4, &tmp1, 1);
   UInt8 tmp2 = sDataSize >> 16;
   memcpy(sPacketData + 5, &tmp2, 1);
   UInt8 tmp3 = sDataSize >> 8;
   memcpy(sPacketData + 6, &tmp3, 1);
   UInt8 tmp4 = sDataSize;
   memcpy(sPacketData + 7, &tmp4, 1);

   blobQueue.push(Blob((UInt8*)sPacketData, 8, blobQueue.size()));

   mBluetoothHandler->dataReceived();
}

void CMessage::generateSingleMessage(UInt8 protocolVersion,
                                     UInt8 serviceType,
                                     UInt8 sessionID,
                                     std::string payload)
{
   UInt32 headerSize = 8;
   if (protocolVersion == 0x02)
      headerSize = 12;

   sVersion        = protocolVersion;
   sCompressedFlag = 0;
   sFrameType      = 0x01; //Single
   sServiceType    = serviceType;
   sFrameData      = 0x00; //Single Frame
   sSessionID      = sessionID;
   sDataSize       = payload.length();//' + 1; //
   sMessageID      = 12345;//rand() % 0xFFFFFFFF + 1;

   dispayField();

   sPacketData = malloc(headerSize + sDataSize);

   UInt8 firstByte = ( (sVersion << 4) & 0xF0 )
                   | ( (sCompressedFlag << 3) & 0x08)
                   | (sFrameType & 0x07);

   UInt32 offset = 0;
   memcpy(sPacketData + offset++, &firstByte, 1);
   memcpy(sPacketData + offset++, &sServiceType, 1);
   memcpy(sPacketData + offset++, &sFrameData, 1);
   memcpy(sPacketData + offset++, &sSessionID, 1);

   UInt8 tmp = sDataSize >> 24;
   memcpy(sPacketData + offset++, &tmp, 1);
   tmp = sDataSize >> 16;
   memcpy(sPacketData + offset++, &tmp, 1);
   tmp = sDataSize >> 8;
   memcpy(sPacketData + offset++, &tmp, 1);
   tmp = sDataSize;
   memcpy(sPacketData + offset++, &tmp, 1);


   if (protocolVersion == 0x02)
   {
       UInt8 tmp1 = sMessageID >> 24;
       memcpy(sPacketData + offset++, &tmp1, 1);
       tmp1 = sMessageID >> 16;
       memcpy(sPacketData + offset++, &tmp1, 1);
       tmp1 = sMessageID >> 8;
       memcpy(sPacketData + offset++, &tmp1, 1);
       tmp1 = sMessageID;
       memcpy(sPacketData + offset++, &tmp1, 1);
   }

   memcpy(sPacketData + offset, (void*)const_cast<char*>(payload.c_str()), sDataSize);

   std::cout << "SINGLE MESSAGE GENERATED: "
             << std::string((char*)(sPacketData + offset), sDataSize) <<  std:: endl;

   blobQueue.push(Blob((UInt8*)sPacketData, offset + sDataSize, blobQueue.size()));

   mBluetoothHandler->dataReceived();
}

void CMessage::generateFinalMessage(UInt8 protocolVersion,
                                    UInt8 serviceType,
                                    UInt8 sessionID,
                                    UInt32 hashCode)
{
   UInt32 headerSize = 8;
   if (protocolVersion == 0x02)
      headerSize = 12;

   sVersion        = protocolVersion;
   sCompressedFlag = 0;
   sFrameType      = 0x00; //Control frame
   sServiceType    = serviceType;
   sFrameData      = 0x04; //End session
   sSessionID      = sessionID;
   sDataSize       = 0x00;
   sMessageID      = hashCode;

   dispayField();

   sPacketData = malloc(headerSize);

   UInt8 firstByte = ( (sVersion << 4) & 0xF0 )
                   | ( (sCompressedFlag << 3) & 0x08)
                   | (sFrameType & 0x07);

   memcpy(sPacketData, &firstByte, 1);
   memcpy(sPacketData + 1, &sServiceType, 1);
   memcpy(sPacketData + 2, &sFrameData, 1);
   memcpy(sPacketData + 3, &sSessionID, 1);

   UInt8 tmp = sDataSize >> 24;
   memcpy(sPacketData + 4, &tmp, 1);
   tmp = sDataSize >> 16;
   memcpy(sPacketData + 5, &tmp, 1);
   tmp = sDataSize >> 8;
   memcpy(sPacketData + 6, &tmp, 1);
   tmp = sDataSize;
   memcpy(sPacketData + 7, &tmp, 1);

   if (protocolVersion == 0x02)
   {
       UInt8 tmp1 = sMessageID >> 24;
       memcpy(sPacketData + 8, &tmp1, 1);
       tmp1 = sMessageID >> 16;
       memcpy(sPacketData + 9, &tmp1, 1);
       tmp1 = sMessageID >> 8;
       memcpy(sPacketData + 10, &tmp1, 1);
       tmp1 = sMessageID;
       memcpy(sPacketData + 11, &tmp1, 1);
   }

  blobQueue.push(Blob((UInt8*)sPacketData, headerSize, blobQueue.size()));

  mBluetoothHandler->dataReceived();
}

void CMessage::generateMultipleMessages(UInt8 protocolVersion,
                                        UInt8 serviceType,
                                        UInt8 sessionID,
                                        std::string payload,
                                        int messagesQuantity)
{
    int headerSize = 8;
    if (protocolVersion == 0x02)
        headerSize = 12;

   sMessageID      = 54321;

   for(int i = 0; i < messagesQuantity; i++)
   {
      sVersion        = protocolVersion;
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
         totalConsecutivePayloadSize = (payload.length()/* + 1*/) * numberOfConsecutiveFrames;

         sDataSize = 0x08;
      }
      else
      {
         sDataSize       = payload.length()/* + 1*/;
      }

      dispayField();

      sPacketData = malloc(headerSize + sDataSize);

      UInt8 firstByte = ( (sVersion << 4) & 0xF0 )
                        | ( (sCompressedFlag << 3) & 0x08)
                        | (sFrameType & 0x07);

      UInt32 offset = 0;
      memcpy(sPacketData + offset++, &firstByte, 1);
      memcpy(sPacketData + offset++, &sServiceType, 1);
      memcpy(sPacketData + offset++, &sFrameData, 1);
      memcpy(sPacketData + offset++, &sSessionID, 1);

      UInt8 tmp1 = sDataSize >> 24;
      memcpy(sPacketData + offset++, &tmp1, 1);
      tmp1 = sDataSize >> 16;
      memcpy(sPacketData + offset++, &tmp1, 1);
      tmp1 = sDataSize >> 8;
      memcpy(sPacketData + offset++, &tmp1, 1);
      tmp1 = sDataSize;
      memcpy(sPacketData + offset++, &tmp1, 1);

      if (protocolVersion == 0x02)
      {
          UInt8 tmp2 = sMessageID >> 24;
          memcpy(sPacketData + offset++, &tmp2, 1);
          tmp2 = sMessageID >> 16;
          memcpy(sPacketData + offset++, &tmp2, 1);
          tmp2 = sMessageID >> 8;
          memcpy(sPacketData + offset++, &tmp2, 1);
          tmp2 = sMessageID;
          memcpy(sPacketData + offset++, &tmp2, 1);
      }

      if(0 != i)
      {
         memcpy(sPacketData + offset, (void*)const_cast<char*>(payload.c_str()), sDataSize);
      }
      else
      {
         UInt8 tmp = totalConsecutivePayloadSize >> 24;
         memcpy(sPacketData + offset++, &tmp, 1);
         tmp = totalConsecutivePayloadSize >> 16;
         memcpy(sPacketData + offset++, &tmp, 1);
         tmp = totalConsecutivePayloadSize >> 8;
         memcpy(sPacketData + offset++, &tmp, 1);
         tmp = totalConsecutivePayloadSize;
         memcpy(sPacketData + offset++, &tmp, 1);

         tmp = numberOfConsecutiveFrames >> 24;
         memcpy(sPacketData + offset++, &tmp, 1);
         tmp = numberOfConsecutiveFrames >> 16;
         memcpy(sPacketData + offset++, &tmp, 1);
         tmp = numberOfConsecutiveFrames >> 8;
         memcpy(sPacketData + offset++, &tmp, 1);
         tmp = numberOfConsecutiveFrames;
         memcpy(sPacketData + offset++, &tmp, 1);
      }


      blobQueue.push(Blob((UInt8*)sPacketData, headerSize + sDataSize, blobQueue.size()));

      mBluetoothHandler->dataReceived();
   }
}

UInt32 CMessage::verify(ProtocolPacketHeader& header, UInt32 fieldsToValidate)
{
   UInt32 returnValue;

   //Get zero bit. Corresponds to version field
   if(getBit(fieldsToValidate, 0))
   {
      if(header.version == sentHeader->version)
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
      if(header.compress == sentHeader->compress)
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
      if(header.frameType == sentHeader->frameType)
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
      if(header.serviceType == sentHeader->serviceType)
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
      if(header.frameData == sentHeader->frameData)
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
      if(header.sessionID == sentHeader->sessionID)
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
      if(header.dataSize == sentHeader->dataSize)
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

Blob CMessage::getNextBlob()
{
   return blobQueue.front();
}

void CMessage::releaseCurrentBlob(const Blob& blob)
{
   free(blob.buffer());

   blobQueue.pop();
}

CMessage::~CMessage()
{}

void CMessage::initBluetooth(IBluetoothHandler * pHandler)
{
    mBluetoothHandler = pHandler;
}

void CMessage::deinitBluetooth()
{}

const Blob CMessage::getBuffer()
{
   return getNextBlob();
}

void CMessage::releaseBuffer(const Blob& blob)
{
   releaseCurrentBlob(blob);
}

void CMessage::sendBuffer(UInt8 * pBuffer, size_t size)
{
    UInt8 firstByte = 0;
    memcpy(&firstByte, pBuffer, sizeof(UInt8) );

    UInt8 version = firstByte >> 4;
    UInt8 headerSize = 8;
    if (version == 2)
        headerSize = 12;

    if ( ((int)size - (int)headerSize) > 0)
    {
        UInt8 *pay = new UInt8[size - headerSize];
        memcpy(pay, pBuffer + headerSize, size - headerSize);
        std::cout << "CMessage::sendBuffer() SEND DATA PAYLOAD: " << std::string( ( (char*)pay), size - headerSize) <<  "\n SIZE : "
                  << std::dec << (size - headerSize) << std::endl;

        delete [] pay;
    }
}


} //namespace AxisCore

