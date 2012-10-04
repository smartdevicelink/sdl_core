#include <string>
#include <map>


class MobileRPCMessage
{
public:
    enum MessageType { REQUEST = 0x0, RESPONSE = 0x1, NOTIFICATION = 0x2, UNDEFINED };
    typedef std::map<std::string,std::string> Parameters;

public:
    MobileRPCMessage( unsigned int protocolVersion, MessageType messageType, 
        unsigned int correlationID = 0 );
    MobileRPCMessage( unsigned int protocolVersion, MessageType messageType, 
        unsigned int correlationID, Parameters params, std::string functionName );
    MobileRPCMessage( unsigned int protocolVersion, MessageType messageType, 
        unsigned int correlationID, Parameters params,
        unsigned int functionID, unsigned int messageSize );
    MobileRPCMessage( unsigned int protocolVersion, MessageType messageType, 
        unsigned int correlationID, Parameters params, std::string functionName,
        unsigned int functionID, unsigned int messageSize );
    ~MobileRPCMessage();

    void setParameters( Parameters params );
    void addParameter( std::string key, std::string value );
    void changeParameter( std::string key, std::string newValue );

    Parameters getParameters() const;
    std::string getParameter( std::string key ) const;

    unsigned int getProtocolVersion() const;
    MessageType getMessageType() const;
    unsigned int getCorrelationID() const;
    const std::string & getFunctionName() const;
    unsigned int getFunctionID() const;
    unsigned int getJSONMessageSize() const;
    

protected:
    /* data */
    unsigned int mProtocolVersion;
    MessageType mMessageType;
    unsigned int mCorrelationID;
    Parameters mParameters;
    std::string mFunctionName;
    unsigned int mFunctionID;
    unsigned int mJSONMessageSize;
};