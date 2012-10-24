#ifndef ALRPCRESPONSE_INCLUDE
#define ALRPCRESPONSE_INCLUDE

#include <string>

#include "ALRPCMessage.hh"

class ALRPCResponse : public ALRPCMessage
{
public:
    ALRPCResponse( unsigned int protocolVersion );
    ALRPCResponse( unsigned int protocolVersion, int methodID );
    ALRPCResponse(void);
    virtual ~ALRPCResponse();

    virtual unsigned int getCorrelationID() const;
    virtual void setCorrelationID( unsigned int correlationID );

private:
    /* data */
    unsigned int 	mCorrelationID;

};

#endif
