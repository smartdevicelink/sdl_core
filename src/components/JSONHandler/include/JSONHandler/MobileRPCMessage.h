#ifndef MOBILERPCMESSAGE_CLASS
#define MOBILERPCMESSAGE_CLASS 

#include <string>
#include <map>


class MobileRPCMessage
{
public:
    enum MessageType { REQUEST = 0x0, RESPONSE = 0x1, NOTIFICATION = 0x2, UNDEFINED };
    typedef std::map<std::string,std::string> Parameters;

public:
    MobileRPCMessage( unsigned int protocolVersion, MessageType messageType );
    MobileRPCMessage( unsigned int protocolVersion, MessageType messageType, 
        std::string functionName );
    MobileRPCMessage( unsigned int protocolVersion, MessageType messageType,
        unsigned int functionID );
    MobileRPCMessage( unsigned int protocolVersion, MessageType messageType, 
        unsigned int correlationID, Parameters params, std::string functionName,
        unsigned int functionID, unsigned int messageSize );
    virtual ~MobileRPCMessage();

    virtual void setParameters( Parameters params );
    virtual void addParameter( std::string key, std::string value );
    virtual void changeParameter( std::string key, std::string newValue );

    virtual Parameters getParameters() const;
    virtual std::string getParameter( std::string key ) const;

    virtual unsigned int getProtocolVersion() const;
    virtual MessageType getMessageType() const;
    virtual unsigned int getCorrelationID() const;
    virtual const std::string & getFunctionName() const;
    virtual unsigned int getFunctionID() const;
    virtual unsigned int getJSONMessageSize() const;

    virtual void setCorrelationID( unsigned int correlationID );
    virtual void setFunctionName( const std::string & functionName );
    virtual void setFunctionID( unsigned int functionID );
    virtual void setJSONMessageSize( unsigned int messageSize );

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

#endif