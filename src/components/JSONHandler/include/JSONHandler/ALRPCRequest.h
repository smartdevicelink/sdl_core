#ifndef ALRPCREQUEST_INCLUDE
#define ALRPCREQUEST_INCLUDE

#include "ALRPCMessage.h"

/**
  *\namespace AppLinkRPC
  *\brief Namespace for AppLink JSON protocol related functionality.
*/
namespace AppLinkRPC
{
    /**
     * \class ALRPCRequest
     * \brief Base class for AppLink Json request messages.
    */
    class ALRPCRequest : public ALRPCMessage
    {
    public:
        /**
         * \brief Constructor
         * \param protocolVersion Version of AppLink protocol (currently 1,2)
        */
        ALRPCRequest( unsigned int protocolVersion );

        /**
         * \brief Constructor
         * \param protocolVersion Version of AppLink protocol (currently 1,2)
         * \param methodID Id of RPC called by message
        */
        ALRPCRequest( unsigned int protocolVersion, int methodId);

        /**
         * \brief Default Constructor
        */
        ALRPCRequest(void);

        /**
         * \brief Destructor
        */
        virtual ~ALRPCRequest();

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

#endif
