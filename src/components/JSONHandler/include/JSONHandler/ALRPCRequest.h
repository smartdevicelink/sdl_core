#ifndef ALRPCREQUEST_INCLUDE
#define ALRPCREQUEST_INCLUDE

#include "ALRPCMessage.h"


class ALRPCRequest : public ALRPCMessage
{
public:
    ALRPCRequest( unsigned int protocolVersion );
    ALRPCRequest( unsigned int protocolVersion, int methodId);
    ALRPCRequest(void);
    virtual ~ALRPCRequest();

    virtual unsigned int getCorrelationID() const;
    virtual void setCorrelationID( unsigned int correlationID );

private:
    /* data */
    unsigned int 	mCorrelationID;

};


#endif
