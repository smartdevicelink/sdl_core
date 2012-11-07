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

// The real size of the packet to send to the Transport Manager
// Todo: implement receiveing this info from the command line
const unsigned int TranspMgrBuffActualSize = 64;

// The size that is reported in the packet header (can be different from the real size to generate errors)
// Todo: implement receiveing this info from the command line
const unsigned int TranspMgrBuffReportedSize = 64;

// The buffer to send to the Transport Manager
//const unsigned char TranspMgrTestBuff[TranspMgrBuffActualSize] = {0, 0, 0};


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
    void send(void *pData, size_t dataSize);
        
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

int main(int argc, const char *argv[])
{
    CTranspMgrTcpClient client(TranspMgrIPAddress, TranspMgrPort);
    
    try
    {
        client.connect();
    }
    catch (std::string *pError)
    {
        std::cout << *pError << std::endl;
        client.disconnect();
        exit(0);
    }
        
    // Todo: send some data
        
        
    client.disconnect();
 
    return 0;
}


