/**
* \file SDLRPCNotification.hpp
* \brief Class SDLRPCNotification header.
* Copyright (c) 2013 Ford Motor Company
*/


#ifndef ALRPCNOTIFICATION_INCLUDE
#define ALRPCNOTIFICATION_INCLUDE

#include "SDLRPCMessage.h"

/**
 * \namespace NsSmartDeviceLinkRPC
 * \brief Namespace for SmartDeviceLink JSON protocol related functionality.
*/ 
namespace NsSmartDeviceLinkRPC
{
    /**
     * \class SDLRPCNotification
     * \brief Base class for SmartDeviceLink Json notification messages.
     */
    class SDLRPCNotification : public SDLRPCMessage
    {
    public:
        /**
         * \brief Constructor
         * \param protocolVersion Version of protocol (currently 1,2)
        */
        SDLRPCNotification( unsigned int protocolVersion );

        /**
         * \brief Constructor
         * \param protocolVersion Version of protocol (currently 1,2).
         * \param methodID ID of RPC called by message.
        */
        SDLRPCNotification( unsigned int protocolVersion, int methodID);

        /**
         * \brief Default Constructor
        */
        SDLRPCNotification(void);

        /**
         * \brief Destructor
        */
        virtual ~SDLRPCNotification();

    };

}

#endif //  ALRPCNOTIFICATION_INCLUDE
