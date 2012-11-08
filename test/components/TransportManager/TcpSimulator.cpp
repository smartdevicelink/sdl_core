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


// -------------------------------------------------------------------------

// Todo: implement receiveing IP adress from the command line arguments 
const std::string TranspMgrIPAddress("213.130.0.1");

// Todo: implement receiveing IP port from the command line arguments
const int TranspMgrPort = 80;

// The real size of the data to send to the Transport Manager
// Todo: implement receiveing this info from the command line
const unsigned int TranspMgrDataActualSize = 64;

// The size that is reported in the packet header (can be different from the real size to generate errors)
// Todo: implement receiveing this info from the command line
const unsigned int TranspMgrDataReportedSize = 64;

// -------------------------------------------------------------------------

/**
 * @brief The header of the protocol
 */
struct PacketHeader
{
    uint8_t version:4;
    bool compressionFlag:1;
    uint8_t frameType:3;
    uint8_t serviceType;
    uint8_t frameData;
    uint8_t sessionId;
    uint32_t dataSize;
    uint32_t messageId;
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

// ----------------------------------------------------------------------------

int main(int argc, const char *argv[])
{
    CTranspMgrTcpClient client(TranspMgrIPAddress, TranspMgrPort);

    size_t totalBuffSize = sizeof(PacketHeader) + TranspMgrDataActualSize;
    uint8_t *pBuff = new uint8_t[totalBuffSize];
    PacketHeader *pHeader = (PacketHeader*)pBuff;

    pHeader->version = 2;                                   // Protocol Version 2
    pHeader->compressionFlag = false;                       // Uncompressed data
    pHeader->frameType = 1;                                 // Single frame
    pHeader->serviceType = 0x0F;                            // Bulk Data 
    pHeader->frameData = 0;                                 // Reserved
    pHeader->sessionId = 0;                                 // unknown
    pHeader->dataSize = TranspMgrDataReportedSize;          // The size of the data in the packet
    pHeader->messageId = 0;                                 // unknown

    // just fill the data with the single value
    memset(&(pBuff[sizeof(PacketHeader)]), 0xFF, TranspMgrDataActualSize);

    try
    {
        client.connect();

        client.send(pBuff, totalBuffSize);    
    }
    catch (std::string *pError)
    {
        std::cout << *pError << std::endl;
    }
        
    client.disconnect();
 
    return 0;
}


