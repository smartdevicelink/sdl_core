#ifndef MOBILERPCMESSAGE_CLASS
#define MOBILERPCMESSAGE_CLASS 

#include <string>

class MobileRPCMessage
{
public:
    enum MessageType { REQUEST = 0x0, RESPONSE = 0x1, NOTIFICATION = 0x2, UNDEFINED };

public:
    MobileRPCMessage( unsigned int protocolVersion, MessageType messageType );
    MobileRPCMessage( unsigned int protocolVersion, MessageType messageType, 
        std::string functionName );
    MobileRPCMessage( unsigned int protocolVersion, MessageType messageType,
        unsigned int functionID );
    MobileRPCMessage( unsigned int protocolVersion, MessageType messageType, 
        unsigned int correlationID, std::string functionName,
        unsigned int functionID, unsigned int messageSize );
    virtual ~MobileRPCMessage();

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

    virtual void setOriginalString( std::string originalString );
    virtual std::string getOriginalString() const;

protected:
    /* data */
    unsigned int mProtocolVersion;
    MessageType mMessageType;
    unsigned int mCorrelationID;
    std::string mFunctionName;
    unsigned int mFunctionID;
    unsigned int mJSONMessageSize;

    std::string mOriginalString;
};

#endif