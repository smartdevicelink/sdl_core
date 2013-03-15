/**
* \file ProtocolPacket.h
* \brief ProtocolPacket class header file.
* 
* Copyright (c) 2013 Ford Motor Company
*/

#ifndef PROTOCOLPACKET_CLASS
#define PROTOCOLPACKET_CLASS

/**
  *\namespace NsProtocolHandler
  *\brief Namespace for SmartDeviceLink ProtocolHandler related functionality.
*/
namespace NsProtocolHandler
{
/**
      *\brief Size of protocol header for first version of protocol.
    */    
const unsigned char PROTOCOL_HEADER_V1_SIZE = 8;

/**
  *\brief Size of protocol header for second version of protocol.
*/
const unsigned char PROTOCOL_HEADER_V2_SIZE = 12;

/**
  *\brief Constant: number of protocol version (1).
*/
const unsigned char PROTOCOL_VERSION_1 = 0x01;

/**
  *\brief Constant: number of protocol version (2).
*/
const unsigned char PROTOCOL_VERSION_2 = 0x02;

/**
  *\brief Constant: flag of no compression
*/
const bool COMPRESS_OFF = false;

/**
  *\brief Constant: Control type of frame used in protocol header.
*/
const unsigned char FRAME_TYPE_CONTROL = 0x00;

/**
  *\brief Constant: Single type of frame used in protocol header.
*/
const unsigned char FRAME_TYPE_SINGLE = 0x01;

/**
  *\brief Constant: First frame for multiple frames used in protocol header.
*/
const unsigned char FRAME_TYPE_FIRST = 0x02;

/**
  *\brief Constant: Consecutive type of frame for multiple frames used in protocol header.
*/
const unsigned char FRAME_TYPE_CONSECUTIVE = 0x03;

/**
  *\brief Constant: RPC type of session 
*/
const unsigned char SERVICE_TYPE_RPC = 0x07;

/**
  *\brief Constant: Bulk data type of session
*/
const unsigned char SERVICE_TYPE_BULK = 0x0F;

/**
  *\brief Unused: If FRAME_TYPE_CONTROL: Constant: Frame is for hear beat.
*/
const unsigned char FRAME_DATA_HEART_BEAT = 0x00;   //Unused

/**
  *\brief If FRAME_TYPE_CONTROL: Constant: Start session frame
*/
const unsigned char FRAME_DATA_START_SESSION = 0x01;

/**
  *\brief If FRAME_TYPE_CONTROL: Constant: Start session acknowledgement frame
*/
const unsigned char FRAME_DATA_START_SESSION_ACK = 0x02;

/**
  *\brief If FRAME_TYPE_CONTROL: Constant: Start session not acknowledgement frame
*/
const unsigned char FRAME_DATA_START_SESSION_NACK = 0x03;

/**
  *\brief If FRAME_TYPE_CONTROL: Constant: End session request frame
*/
const unsigned char FRAME_DATA_END_SESSION = 0x04;

/**
  *\brief If FRAME_TYPE_CONTROL: Constant: End session not acknowledgement frame
*/
const unsigned char FRAME_DATA_END_SESSION_NACK = 0x05; //Assumption

/**
  *\brief If FRAME_TYPE_CONTROL: Constant: Maximum of consecutive frame numbers 
  * after which count has to roll over to 0x01
*/
const unsigned char FRAME_DATA_MAX_VALUE = 0xFF;

/**
  *\brief If FRAME_TYPE_CONTROL: Constant: Maximum size of one frame excluding frame header.
*/
const unsigned int MAXIMUM_FRAME_DATA_SIZE = 1488;

/**
  *\brief If FRAME_TYPE_CONSECUTIVE: Constant: Marks last frame in mutliframe message.
*/
const unsigned char FRAME_DATA_LAST_FRAME = 0x00;

/**
  *\brief If FRAME_TYPE_CONSECUTIVE: Constant: Size of first frame in mutliframe message.
*/
const unsigned char FIRST_FRAME_DATA_SIZE = 0x08;

/**
 *\enum RESULT_CODE
 *\brief Return type for operations with message handling.
*/
enum RESULT_CODE
{
   RESULT_OK                     = 1,
   RESULT_FAIL                   = 2,
   RESULT_NUMBER_BUSY            = 3,
   RESULT_CONNECTION_LOST        = 4,
   RESULT_DISCONNECTED           = 5,
   RESULT_NOTFOUND               = 6,
   RESULT_TIMEOUT                = 7,
   RESULT_WRONG_SEQUENCE         = 8,
   RESULT_AGAIN                  = 9,
   RESULT_WRONG_FRAME_SEQUENCE   = 10,
   RESULT_IN_PROGRESS            = 11,
   RESULT_REASSIGN               = 12,
   RESULT_XML_PARSING            = 13,
   RESULT_RESEND_ACK             = 14,
   RESULT_DEFRERRED               = 15,
   RESULT_UNKNOWN                = 255
};

/**
 * \struct ProtocolHeader
 * \brief Used for storing protocol header of a message.
*/
struct ProtocolHeader
{
    /**
     * \brief Constructor
    */
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

    /**
      *\brief Version of protocol
    */
    unsigned char version;

    /**
      *\brief Compression flag
    */
    bool compress;

    /**
      *\brief Type of frame (Single/First/Consecutive)
    */
    unsigned char frameType;

    /**
      *\brief Type of service (RPC/Buld data)
    */
    unsigned char serviceType;

    /**
      *\brief Data about frame (Start/End session etc)
    */
    unsigned char frameData;

    /**
      *\brief Session number withing connection
    */
    unsigned char sessionID;

    /**
      *\brief Size of message itself, excluding size of header
    */
    unsigned int dataSize;
   
    /**
      *\brief Used only in second versin of protocol: Message ID
    */
    unsigned int messageId;
};

/**
 * \struct ProtocolData
 * \brief Used for storing message and its size.
*/
struct ProtocolData
{
    ProtocolData() :
        data(0),
        totalDataBytes(0x00)
    {}

    unsigned char * data;
    unsigned int totalDataBytes;
};

    /**
     * \class ProtocolPacket
     * \brief Class for forming/parsing protocol headers of the message and
     * handling multiple frames of the message.
    */
    class ProtocolPacket
    {
    public:
        /**
         * \brief Default constructor
        */
        ProtocolPacket();

        /**
         * \brief Constructor
         * \param version Version of protocol
         * \param compress Compression flag
         * \param frameType Type of frame (Single/First/Consecutive)
         * \param serviceType Type of session (RPC/Bulk data)
         * \param frameData Information about frame: start/end session, number of frame, etc
         * \param sessionID Number of frame within connection
         * \param dataSize Size of message string
         * \param messageID ID of message or hash code - only for second protocol
         * \param data Message string if provided
        */
        ProtocolPacket(unsigned char version,
                             bool compress,
                             unsigned char frameType,
                             unsigned char serviceType,
                             unsigned char frameData,
                             unsigned char sessionID,
                             unsigned int dataSize,
                             unsigned int messageID,
                             const unsigned char * data = 0);
        /**
         * \brief Destructor
        */
        ~ProtocolPacket();

        /*Serialization*/
        /**
         * \brief Serializes info about message into protocol header.
         * \param version Version of protocol
         * \param compress Compression flag
         * \param frameType Type of frame (Single/First/Consecutive)
         * \param serviceType Type of session (RPC/Bulk data)
         * \param frameData Information about frame: start/end session, number of frame, etc
         * \param sessionID Number of frame within connection
         * \param dataSize Size of message string
         * \param messageID ID of message or hash code - only for second protocol
         * \param data Message string if provided
         * \return \saRESULT_CODE Status of serialization
        */
        RESULT_CODE serializePacket(unsigned char version,
                             bool compress,
                             unsigned char frameType,
                             unsigned char serviceType,
                             unsigned char frameData,
                             unsigned char sessionID,
                             unsigned int dataSize,
                             unsigned int messageID,
                             const unsigned char * data = 0);

        /**
         * \brief Appends message frame to existing message in 
         * recieving multiframe messages.
         * \param chunkData Current frame's message string
         * \param chunkDataSize Size of current message string
         * \return \saRESULT_CODE Status of serialization
         */
        RESULT_CODE appendData( unsigned char * chunkData, 
                            unsigned int chunkDataSize );

        /**
         * \brief Getter of serialized message with protocol header
         * \return unsigned char * Message string or 0 if not serialized properly.
         */
        unsigned char * getPacket() const;

        /**
         * \brief Getter of message size including protocol header
         * \return unsigned int size of message string
         */
        unsigned int getPacketSize() const;
        /*End of Serialization*/

        /*Deserialization*/

        /**
         * \brief Parses protocol header
         * \param message Incoming message string containing both header and message body
         * \param messageSize Incoming message size
         * \return \saRESULT_CODE Status of serialization
         */
        RESULT_CODE deserializePacket(const unsigned char * message, unsigned int messageSize);

        /**
         * \brief Getter of protocol version.
         */
        unsigned char getVersion() const;

        /**
         * \brief Getter of compression flag
         */
        bool getIfCompress() const;

        /**
         * \brief Getter of frame type (single/first/etc)
         */
        unsigned char getFrameType() const;

        /**
          *\brief Getter of service type (RPC/Bulk data)
        */
        unsigned char getServiceType() const;

        /**
          *\brief Getter of frame data (start/end session, number of frame etc)
        */
        unsigned char getFrameData() const;

        /**
          *\brief Getter of session number
        */
        unsigned char getSessionId() const;

        /**
          *\brief Getter of size of message body
        */
        unsigned int getDataSize() const;

        /**
          *\brief Getter of message id for second version of protocol
        */
        unsigned int getMessageId() const;

        /**
          *\brief Getter of message string
        */
        unsigned char * getData() const;

        /**
          *\brief Setter for size of multiframe message
        */
        void setTotalDataBytes(unsigned int dataBytes);

        /**
          *\brief Getter for size of multiframe message
        */
        unsigned int getTotalDataBytes() const;
        /*End of Deserialization*/         

    private:
        /**
          *\brief Serialized message string
        */
        unsigned char * mPacket;

        /**
          *\brief Serialized message string size
        */
        unsigned int mTotalPacketSize;

        /**
          *\brief Protocol header
        */
        ProtocolHeader mPacketHeader;

        /**
          *\brief Message body
        */
        ProtocolData mPacketData;

        /**
          *\brief Offset for multiframe messages
        */
        unsigned int mDataOffset;
    };
}

#endif // PROTOCOLPACKET_CLASS