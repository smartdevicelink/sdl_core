#ifndef ALRPCNOTIFICATION_INCLUDE
#define ALRPCNOTIFICATION_INCLUDE

#include "ALRPCMessage.h"

class ALRPCNotification : public ALRPCMessage
{
public:
    ALRPCNotification( unsigned int protocolVersion );
    ALRPCNotification( unsigned int protocolVersion, int methodID);
    ALRPCNotification(void);
    virtual ~ALRPCNotification();

    /* data */
};

#endif
