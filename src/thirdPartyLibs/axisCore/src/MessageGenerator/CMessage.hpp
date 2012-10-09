#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

typedef unsigned char   UInt8;
typedef unsigned int    UInt32;

class CMessage
{
public:
   static void write();

private:
   static void generate();

   static void generateInitialMessage();
   static void generateSingleMessage();
   static void generateFinalMessage();

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

#endif /* MESSAGE_HPP_ */
