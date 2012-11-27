#ifndef PROTOCOLPACKET_CLASS
#define PROTOCOLPACKET_CLASS

namespace NsProtocolHandler
{
const unsigned char PROTOCOL_HEADER_V1_SIZE = 8;
const unsigned char PROTOCOL_HEADER_V2_SIZE = 12;

const unsigned char PROTOCOL_VERSION_1 = 0x01;
const unsigned char PROTOCOL_VERSION_2 = 0x02;

const bool COMPRESS_OFF = false;

const unsigned char FRAME_TYPE_CONTROL = 0x00;
const unsigned char FRAME_TYPE_SINGLE = 0x01;
const unsigned char FRAME_TYPE_FIRST = 0x02;
const unsigned char FRAME_TYPE_CONSECUTIVE = 0x03;

const unsigned char SERVICE_TYPE_RPC = 0x07;
const unsigned char SERVICE_TYPE_BULK = 0x0F;

/**
 * If FRAME_TYPE_CONTROL :
 */
const unsigned char FRAME_DATA_HEART_BEAT = 0x00;   //Unused
const unsigned char FRAME_DATA_START_SESSION = 0x01;
const unsigned char FRAME_DATA_START_SESSION_ACK = 0x02;
const unsigned char FRAME_DATA_START_SESSION_NACK = 0x03;
const unsigned char FRAME_DATA_END_SESSION = 0x04;
const unsigned char FRAME_DATA_END_SESSION_NACK = 0x05; //Assumption

const unsigned char FRAME_DATA_MAX_VALUE = 0xFF;

const unsigned int MAXIMUM_FRAME_SIZE = 1024;

/**
 * If FRAME_TYPE_CONSECUTIVE :
 */
const unsigned char FRAME_DATA_LAST_FRAME = 0x00;

const unsigned char FIRST_FRAME_DATA_SIZE = 0x08;

    struct ProtocolHeader
    {
        ProtocolHeader() :
            version(0x00),
            compress(0x00),
            frameType(0x00),
            serviceType(0x00),
            frameData(0x00),
            sessionID(0x00),
            dataSize(0x00),
            messageId(0x00)
        {}

        unsigned char version;
        bool compress;
        unsigned char frameType;
        unsigned char serviceType;
        unsigned char frameData;
        unsigned char sessionID;
        unsigned int dataSize;
        /**
          * MessageID is used only in protocol version 2
          */
        unsigned int messageId;
    };

    struct ProtocolData
    {
        ProtocolData() :
            data(0),
            totalDataBytes(0x00),
            dataOffset(0x00)
        {}

        unsigned char * data;
        unsigned int totalDataBytes;
    };

    class ProtocolPacket
    {
    public:
        ProtocolPacket();
        ~ProtocolPacket();

        /*Serialization*/
        RESULT_CODE serializePacket(unsigned char version,
                             bool compress,
                             unsigned char frameType,
                             unsigned char serviceType,
                             unsigned char frameData,
                             unsigned char sessionID,
                             unsigned int dataSize,
                             unsigned int messageID,
                             const unsigned char * data = 0);
        RESULT_CODE appendData( unsigned char * chunkData, 
                            unsigned int chunkDataSize );
        unsigned char * getPacket() const;
        unsigned int getPacketSize() const;
        /*End of Serialization*/

        /*Deserialization*/
        RESULT_CODE deserializePacket(unsigned int message, unsigned int messageSize);
        void pushConsecutiveFrame();
        unsigned char getVersion() const;
        bool getIfCompress() const;
        unsigned char getFrameType() const;
        unsigned char getServiceType() const;
        unsigned char getFrameData() const;
        unsigned char getSessionId() const;
        unsigned int getDataSize() const;
        unsigned int getMessageId() const;
        unsigned char * getData() const;
        void setTotalDataBytes(unsigned int dataBytes);
        /*End of Deserialization*/         

    private:
        unsigned char * mPacket;
        unsigned int mTotalPacketSize;

        unsigned char * mHeader;
        unsigned int mHeaderSize;
         
        ProtocolHeader mPacketHeader;
        ProtocolData mPacketData;


        unsigned int mDataOffset;
    };
}

#endif // PROTOCOLPACKET_CLASS