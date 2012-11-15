// TcpSimulator.cpp
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <getopt.h>


// -------------------------------------------------------------------------

/**
 * @brief Confituration of the application
 */
struct Config
{
    std::string ipAddress;
    int port;
    int protocolVersion;
    uint32_t actualDataSize;
    uint32_t reportedDataSize;    
};

/**
 * @brief The common part of the header for both protocol versions 
 */
struct PacketHeaderBase
{
    uint8_t version:4;
    bool compressionFlag:1;
    uint8_t frameType:3;
    uint8_t serviceType;
    uint8_t frameData; 
    uint8_t sessionId;
    uint32_t dataSize;
};

/**
 * @brief The header of the protocol version 1
 */
struct PacketHeaderV1
{
    PacketHeaderBase base;

    // no additional fields
};

/**
 * @brief The header of the protocol version 2
 */
struct PacketHeaderV2
{
    PacketHeaderBase base;

    uint32_t messageId;
};

// -------------------------------------------------------------------------
/**
 * @brief Default config values
 */
const Config DefaultConfigValues = 
{
    std::string("127.0.0.1"),       // Default IP address
    12345,                          // Default TCP port
    2,                              // Default protocol version
    32,                             // Default actual data size
    32,                             // Default reported data size
};

// -------------------------------------------------------------------------

/**
 * @brief The class incapsulates connection routines to the TCP socket of the Transport Manager
 */
class CTranspMgrTcpClient
{
public:
 
    /**
     * @brief CTranspMgrTcpClient receives all necesary parameters via this constructor 
     * 
     * @param ipAddress The IP address of the host where Transport Manager is running
     * @param port The TCP port of Transport Manager
     */
    CTranspMgrTcpClient(const std::string ipAddress, const int port);
    
    /**
     * @brief Destructor closes the socket if necessary
     */
    ~CTranspMgrTcpClient();
 
    /**
     * @brief Tries to establish TCP connection to the Transport Manager
     */
    void connect();
    
    /**
     * @brief Closes the socket (if it was opened before)
     */
    void disconnect();
    
    /**
     *  @brief Sends data packet to the Transport Manager
     *
     *  @brief pData Pointer to the data to send
     *  @brief dataSize Size of the buffer to send 
     */
    void send(const void *pData, size_t dataSize);
        
private:
    int mPort;
    std::string mIpAddress;
    
    int mSocketFd;                    // socket file descriptor
};

// ----------------------------------------------------------------------------

CTranspMgrTcpClient::CTranspMgrTcpClient(const std::string ipAddress, const int port) 
    : mPort(port)
    , mIpAddress(ipAddress)
    , mSocketFd(-1)
{   
}

// ----------------------------------------------------------------------------

CTranspMgrTcpClient::~CTranspMgrTcpClient()
{
    if (mSocketFd != -1)
    {
        close(mSocketFd);
    }
}

// ----------------------------------------------------------------------------

void CTranspMgrTcpClient::connect()
{    
    struct hostent *server;
    struct sockaddr_in serverAddr;
    
    mSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (mSocketFd < 0) 
    {
        throw new std::string("Error opening socket\n");
    } 
    
    server = gethostbyname(mIpAddress.c_str());
    if (server == NULL) 
    {
        throw new std::string("Error, no such host\n");
    }  
    
    bzero( (char*) &serverAddr, sizeof(serverAddr) );
    serverAddr.sin_family = AF_INET;   
    bcopy((char*)server->h_addr, (char*)&serverAddr.sin_addr.s_addr, server->h_length);
    serverAddr.sin_port = htons(mPort);
    
    if (::connect(mSocketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        throw new std::string("Error connecting\n");
    }
}

// ----------------------------------------------------------------------------

void CTranspMgrTcpClient::disconnect()
{
    close(mSocketFd);
    
    mSocketFd = -1;
}

// ----------------------------------------------------------------------------

void CTranspMgrTcpClient::send(const void* pData, size_t dataSize)
{
    if (pData != NULL)
    {
        ssize_t written = write(mSocketFd, pData, dataSize);
        
        if (written == -1)
        {
            throw new std::string("Error writing into the socket");
        }
        if (written != dataSize)
        {
            std::cout << "Warning: expected to send " << dataSize << 
                " bytes, but sent " << written << " bytes\n";
        }
    }
}

// -------------------------------------------------------------------------

static void printUsage(const std::string &programName)
{
    std::cout << "Usage: " << programName << " [OPTION]" << std::endl;
    std::cout << "Send test packet to the TransportManager in order to test TCP Adapter" << std::endl;
    std::cout << std::endl;
    std::cout << "-i, --ip IP_ADDRESS           IP address where to send the packet (Default: " 
        << DefaultConfigValues.ipAddress << ")" << std::endl;

    std::cout << "-p, --port PORT_NUMBER        TCP port number (Default: " 
        << DefaultConfigValues.port << ")" << std::endl;

    std::cout << "-v, --version [1 | 2]         Protocol version. (Default: " 
        << DefaultConfigValues.protocolVersion << ")" << std::endl;

    std::cout << "-a, --actual-size SIZE        The real size of the data in the packet to send (Default: "
        << DefaultConfigValues.actualDataSize << ")" << std::endl;
    
    std::cout << "-r, --reported-size SIZE      The size of the packet which is written in the packet header (Default: " 
        << DefaultConfigValues.reportedDataSize << ")" << std::endl;

    std::cout << "-h, --help                    Print this help" << std::endl << std::endl;
}

// -------------------------------------------------------------------------

static bool initConfig(int argc, char **argv, Config *pConfig)
{
    bool result = true;
   
    struct option long_options[] =
    {
        {"help", no_argument, 0, 'h'},
        {"ip", required_argument, 0, 'i'},
        {"port", required_argument, 0, 'p'},
        {"version", required_argument, 0, 'v'},
        {"actual-size", required_argument, 0, 'a'},
        {"reported-size", required_argument, 0, 'r'},
        {0, 0, 0, 0}
    };

    *pConfig = DefaultConfigValues;

    while (true)
    {
        int option_index = 0;
        int c;
    
        c = getopt_long(argc, argv, "hi:p:v:a:r:", long_options, &option_index);

        if (c == -1)
            break;

        switch (c)
        {
            case 'i':
                if (optarg)
                {
                    // Todo: parse this string for valid IP address
                    pConfig->ipAddress = optarg;
                }
                else
                {
                    std::cout << "Error: IP address is missing" << std::endl;
                    result = false;
                }
                break;

            case 'p':
                if (optarg)
                {
                    pConfig->port = atoi(optarg);
                }
                else 
                {
                    std::cout << "Error: port is missing" << std::endl;
                    result = false;
                }
                break;
 
            case 'v':
                if (optarg)
                {
                    pConfig->protocolVersion = atoi(optarg);
                }
                else 
                {
                    std::cout << "Default protocol version is" << DefaultConfigValues.protocolVersion << std::endl;
                }
                break;
 
            case 'a':
                if (optarg)
                {
                    pConfig->actualDataSize = atoi(optarg);
                }
                else 
                {
                    std::cout << "Error: data size is missing" << std::endl;
                    result = false;
                }
                break;
 
            case 'r':
                if (optarg)
                {
                    pConfig->reportedDataSize = atoi(optarg);
                }
                else 
                {
                    std::cout << "Error: data size is missing" << std::endl;
                    result = false;
                }
                break;

            default:
                result = false;
        }
    }
    
    if (result == false)
    {
        printUsage(argv[0]);
    }

    return result;
}

// -------------------------------------------------------------------------

static uint8_t* makePacket(const Config &config, int &packetSize)
{
    uint8_t *pBuff = 0;
    PacketHeaderBase *pBase;
    size_t headerSize;

    if (config.protocolVersion == 1)
    {
        headerSize = sizeof(PacketHeaderV1);
    }
    else if (config.protocolVersion == 2)
    {
        headerSize = sizeof(PacketHeaderV2);
    }
    else
    {
        throw new std::string("Unsupported protocol version");
    }
    
    packetSize = headerSize + config.actualDataSize;

    pBuff = new uint8_t[packetSize];
    pBase = (PacketHeaderBase*)pBuff;

    pBase->version = config.protocolVersion;
    pBase->compressionFlag = false;
    pBase->frameType = 1;
    pBase->serviceType = 0x0F;
    pBase->frameData = 0;
    pBase->sessionId = 0;
    pBase->dataSize = config.reportedDataSize;            // The size of the data in the packet
 
    if (config.protocolVersion == 2)
    {
        PacketHeaderV2 *pHeader = (PacketHeaderV2*)pBuff;

        pHeader->messageId = 0;
    }
    
    // just fill the data with the single value
    memset(&(pBuff[headerSize]), 0xFF, config.actualDataSize);

    return pBuff;
}

// ----------------------------------------------------------------------------

int main(int argc, char **argv)
{
    Config config;
    uint8_t *pBuff = 0;
    int buffSize;

    if (initConfig(argc, argv, &config) == false)
    {
        return 0;  // can't continue, do not have enough configuration data
    }

    CTranspMgrTcpClient client(config.ipAddress, config.port);
   
    try
    {
        pBuff = makePacket(config, /*out*/buffSize);
        
        client.connect();

        client.send(pBuff, buffSize);    

        std::cout << "The packet has been sent successfully" << std::endl;
    }
    catch (std::string *pError)
    {
        std::cout << *pError << std::endl;
    }
        
    delete pBuff;

    client.disconnect();
 
    return 0;
}


