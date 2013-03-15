/**
* \file SDLRPCRequest.hpp
* \brief Class SDLRPCRequest header.
* Copyright (c) 2013 Ford Motor Company
*/


#ifndef ALRPCREQUEST_INCLUDE
#define ALRPCREQUEST_INCLUDE

#include "SDLRPCMessage.h"

/**
  *\namespace NsSmartDeviceLinkRPC
  *\brief Namespace for SmartDeviceLink JSON protocol related functionality.
*/
namespace NsSmartDeviceLinkRPC
{
    /**
     * \class SDLRPCRequest
     * \brief Base class for SmartDeviceLink Json request messages.
    */
    class SDLRPCRequest : public SDLRPCMessage
    {
    public:
        /**
         * \brief Constructor
         * \param protocolVersion Version of protocol (currently 1,2)
        */
        SDLRPCRequest( unsigned int protocolVersion );

        /**
         * \brief Constructor
         * \param protocolVersion Version of protocol (currently 1,2)
         * \param methodID Id of RPC called by message
        */
        SDLRPCRequest( unsigned int protocolVersion, int methodId);

        /**
         * \brief Default Constructor
        */
        SDLRPCRequest(void);

        /**
         * \brief Destructor
        */
        virtual ~SDLRPCRequest();

        /**
         * \brief Getter for correlation id of JSON message.
         * \return correlation id of JSON message.
         */
        virtual unsigned int getCorrelationID() const;

        /**
         * \brief Setter for correlation id of JSON message.
         * \param correlationID Correlation ID of JSON message.
         * \return type description
         */
        virtual void setCorrelationID( unsigned int correlationID );

    private:
        /**
          *\brief Correlation id of JSON message.
        */
        unsigned int 	mCorrelationID;

    };

}

#endif  // ALRPCREQUEST_INCLUDE
