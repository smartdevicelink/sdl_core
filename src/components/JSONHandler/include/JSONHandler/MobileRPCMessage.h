#ifndef MOBILERPCMESSAGE_CLASS
#define MOBILERPCMESSAGE_CLASS 

#include <string>
#include <json/value.h>

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

    virtual std::string serialize() const;
    virtual int deserialize( const std::string & jsonString );

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

    virtual void setParametersString( std::string parametersString );
    virtual std::string getParametersString() const;

protected:
    std::string jsonToString( const Json::Value & jsonObject ) const;

private:
    /* data */
    unsigned int mProtocolVersion;
    MessageType mMessageType;
    unsigned int mCorrelationID;
    std::string mFunctionName;
    unsigned int mFunctionID;
    unsigned int mJSONMessageSize;

    std::string mParametersString;
};

#endif