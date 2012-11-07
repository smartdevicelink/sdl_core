#ifndef RPCS2_NOTIFICATION_CLASS
#define RPCS2_NOTIFICATION_CLASS

#include "RPC2Command.h"

namespace RPC2Communication
{
    class RPC2Notification : public RPC2Command
    {
    public:
        RPC2Notification( );
        RPC2Notification( int method );
        ~RPC2Notification();

       
    private:

    };
}

#endif

