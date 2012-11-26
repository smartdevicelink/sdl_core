#ifndef TYPES_HPP
#define TYPES_HPP


/**
 * 8-bit signed value
 */
typedef char            Int8;

/**
 * 8-bit unsigned value
 */
typedef unsigned char   UInt8;

/**
 * 16-bit signed value
 */
typedef short           Int16;

/**
 * 16-bit unsigned value
 */
typedef unsigned short  UInt16;

/**
 * 32-bit signed value
 */
typedef int             Int32;

/**
 * 32-bit unsigned value
 */
typedef unsigned int    UInt32;

/**
 * 64-bit unsigned value
 */
typedef unsigned long long UInt64;

/**
 * 64-bit signed value
 */
typedef long long Int64;

/**
 * Number of data source priorities in AXIS
 */
//const UInt8    PRIO_NUM                   = 3;

/**
 * Maximum buffer size
 */
//const UInt32   MAX_SIZE                   = 4096;

/**
 * Number of service channel
 */
//const UInt32   SERVICE_CHANNEL_NUMBER     = 0;

/**
 * Channel supervisor reserved channel
 */
//const UInt32   CS_SERVICE_CHANNEL         = 256;
/**
 * Default port for TCP connection
 */
//const char AXIS_PORT[] = "40000";
//const char AXIS_PORT[] = "45000";

/**
 * Default port for CTCPAutoConnectionFinder
 */
//const int AXIS_TCP_AUTO_CONNECTION_FINDER_BRD_PORT = 49789;
//const int AXIS_TCP_AUTO_CONNECTION_FINDER_BRD_PORT = 45789;


/**
 * Default size of buffer queues for transmission and receiving
 */
//  const UInt32   QUEUE_SIZE     = 200;

/**
* Time before frame must be retransmitted
*/
//const UInt64 RETRANSMIT_TIME_NSEC = 5LLU * 1000 * 1000 * 1000;

/**
 * Data source priorities
 */
/*enum TChannelPriority
{
   eSafetyCritical = 0,
   eRealTime = 1,
   ePlainData = 2
};*/

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
   RESULT_DEFERRED               = 15,
   RESULT_UNKNOWN                = 255
};
/**
 * List of available types of carriers
 */
/*enum ECarrierType
{
   eDummyTcpCarrier,    //!< eDummyTcpCarrier - TCP carrier for testing purposes
   eTcpAutoCarrier,     //!< eTcpAutoCarrier - TCP carrier, detected by CTcpAutoConnectionFinder
   eLastCarrier         //!< eLastCarrier - special value used for compile time counting of carrier types. Must be last in enum.
};

enum CALLBACKTYPE
{
   DATA_RECEIVED,
   BUFF_OVERFLOW,
   CHANN_DELETED,
   CONNECTI_LOST
};*/

#endif // TYPES_HPP
