/**
* \file SDLRPCResponse.h
* \brief Class SDLRPCResponse header.
* Copyright (c) 2013 Ford Motor Company
*/


#ifndef ALRPCRESPONSE_INCLUDE
#define ALRPCRESPONSE_INCLUDE

#include <string>
#include "SDLRPCMessage.h"
#include "JSONHandler/SDLRPCObjects/V2/Result.h"

/**
  *\namespace NsSmartDeviceLinkRPC
  *\brief Namespace for SmartDeviceLink JSON protocol related functionality.
*/
namespace NsSmartDeviceLinkRPC
{
    /**
     * \class SDLRPCResponse
     * \brief Base class for SmartDeviceLink Json response message classes.
    */
    class SDLRPCResponse : public SDLRPCMessage
    {
    public:
        /**
         * \brief Constructor
         * \param protocolVersion Version of protocol in message.
        */
        SDLRPCResponse(unsigned int protocolVersion);

        /**
         * \brief Constructor
         * \param protocolVersion Version of protocol in message.
         * \param methodID ID of RPC called by message.
        */
        SDLRPCResponse(unsigned int protocolVersion, int methodID);

        /**
         * \brief Default Constructor
        */
        SDLRPCResponse(void);

        /**
         * \brief Destructor
        */
        virtual ~SDLRPCResponse();

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
        const NsSmartDeviceLinkRPCV2::Result& get_resultCode(void) const;
        const std::string* get_info(void) const;

        bool set_success(bool success_);
        bool set_resultCode(const NsSmartDeviceLinkRPCV2::Result& resultCode_);

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
        NsSmartDeviceLinkRPCV2::Result resultCode;

        ///  Provides additional human readable info regarding the result.
        std::string* info;    //!< (1000)
    };
}

#endif  // ALRPCRESPONSE_INCLUDE
