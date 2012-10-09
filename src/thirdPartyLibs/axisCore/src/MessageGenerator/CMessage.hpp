#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_


#include <queue>

#include "../../utils/misc/Types.hpp"
#include "../../transport/bt/Blob.hpp"

class CMessage
{
public:
   static Blob getNextBlob();
   static void generateInitialMessage();
   static void generateSingleMessage();
   static void generateFinalMessage();

   //static Blob getCurrentBlob() { return currentBlob; }

   static Blob currentBlob;

private:

   static std::queue<Blob> blobQueue;

   //static void write();
   //static void generate();



   static void dispayField();

   static UInt8 sVersion;
   static UInt8 sCompressedFlag;
   static UInt8 sFrameType;
   static UInt8 sServiceType;
   static UInt8 sFrameData;
   static UInt8 sSessionID;
   static UInt32 sDataSize;
   static UInt32 sMessageID;

   static void* sPacketData;


};
/*
const Blob getBuffer()
{
   return CMessage::getNextBlob();
}
*/
#endif /* MESSAGE_HPP_ */
