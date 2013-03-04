/**
* \file ALRPCResponse.h
* \brief Class ALRPCResponse header.
* \author PVyshnevska
*/


#ifndef ALRPCRESPONSE_INCLUDE
#define ALRPCRESPONSE_INCLUDE

#include <string>
#include "ALRPCMessage.h"
#include "JSONHandler/ALRPCObjects/V2/Result.h"

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
        ALRPCResponse(unsigned int protocolVersion);

        /**
         * \brief Constructor
         * \param protocolVersion Version of AppLink protocol in message.
         * \param methodID ID of RPC called by message.
        */
        ALRPCResponse(unsigned int protocolVersion, int methodID);

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
        virtual void setCorrelationID(unsigned int correlationID);

        bool get_success(void) const;
        const NsAppLinkRPCV2::Result& get_resultCode(void) const;
        const std::string* get_info(void) const;

        bool set_success(bool success_);
        bool set_resultCode(const NsAppLinkRPCV2::Result& resultCode_);

        void reset_info(void);
        bool set_info(const std::string& info_);

    protected:
        /**
          *\brief Correlation Id of Json message.
        */
        unsigned int    mCorrelationID;

        /**
        true, if successful
        false, if failed
        */
        bool success;

        ///  See Result
        NsAppLinkRPCV2::Result resultCode;

        ///  Provides additional human readable info regarding the result.
        std::string* info;    //!< (1000)
    };
}

#endif  // ALRPCRESPONSE_INCLUDE
