/**
* \file ALRPCResponse.h
* \brief Class ALRPCResponse header.
* \author PVyshnevska
*/


#ifndef ALRPCRESPONSE_INCLUDE
#define ALRPCRESPONSE_INCLUDE

#include <string>
#include "ALRPCMessage.h"

/**
  *\namespace NsAppLinkRPC
  *\brief Namespace for AppLink JSON protocol related functionality.
*/
namespace NsAppLinkRPC
{
    /**
     * \class ALRPCResponse
     * \brief Base class for AppLink Json response message classes.
    */
    class ALRPCResponse : public ALRPCMessage
    {
    public:
        /**
         * \brief Constructor
         * \param protocolVersion Version of AppLink protocol in message.
        */
        ALRPCResponse( unsigned int protocolVersion );

        /**
         * \brief Constructor
         * \param protocolVersion Version of AppLink protocol in message.
         * \param methodID ID of RPC called by message.
        */
        ALRPCResponse( unsigned int protocolVersion, int methodID );

        /**
         * \brief Default Constructor
        */
        ALRPCResponse(void);

        /**
         * \brief Destructor
        */
        virtual ~ALRPCResponse();

        /**
         * \brief Getter for correlation id.
         * \return Correlation ID of Json Message
         */
        virtual unsigned int getCorrelationID() const;

        /**
         * \brief Setter for correlation id.
         * \param correlationID Correlation ID of Json message
         */
        virtual void setCorrelationID( unsigned int correlationID );

    private:
        /**
          *\brief Correlation Id of Json message.
        */
        unsigned int 	mCorrelationID;

    };
}

#endif  // ALRPCRESPONSE_INCLUDE
