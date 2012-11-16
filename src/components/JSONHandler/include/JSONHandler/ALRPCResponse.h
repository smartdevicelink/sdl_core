#ifndef ALRPCRESPONSE_INCLUDE
#define ALRPCRESPONSE_INCLUDE

#include <string>
#include "ALRPCMessage.h"

/**
  *\namespace AppLinkRPC
  *\brief Namespace for AppLink JSON protocol related functionality.
*/
namespace AppLinkRPC
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
         * \brief {2: Brief Description}
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

#endif
