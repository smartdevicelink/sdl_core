/**
* \file RPC2Error.hpp
* \brief RPC2Error class header.
* Copyright (c) 2013 Ford Motor Company
*/


#ifndef RPC2COMMUNICATION_RPC2ERROR
#define RPC2COMMUNICATION_RPC2ERROR

#include <string>
#include "RPC2Command.h"

/**
  *\namespace NsRPC2Communication
  *\brief Namespace for communication with HMI.
*/
namespace NsRPC2Communication
{
    /**
     * \class RPC2Error
     * \brief Base class for erros in Json RPC2
    */
    class RPC2Error : public RPC2Command
    {
    public:
        /**
         * \enum Codes
         * \brief Error codes.
        */   
    	enum Codes
    	{
              INVALID_REQUEST=-32600,
              METHOD_NOT_FOUND=-32601,
              INVALID_PARAMS=-32602,
              INTERNAL_ERROR=-32603
    	};

        /**
         * \brief Constructor
        */
        RPC2Error(void);

        /**
         * \brief Copy Constructor
         * \param err RPC2Error instance.
        */
        RPC2Error(const RPC2Error& err);

        /**
         * \brief Constructor
         * \param code Error code from 
         * \sa Codes
         * \param message Error message
         * \param id Method id.
        */
        RPC2Error(int code, const std::string& message,int id);

        /**
         * \brief Constructor
         * \param code Error code from 
         * \sa Codes
         * \param message Error message.
        */
        RPC2Error(int code, const std::string& message);

        /**
         * \brief Destructor
        */
        virtual ~RPC2Error();

        /**
         * \brief Getter of id of the message.
         * \return Id of the message.
         */
        int getId() const;

        /**
         * \brief Setter of id of the message.
         * \param id Id of the message.
         * \return type description
         */
        void setId(const int id);

        /**
         * \brief Resets id of the message.
         * TODO why do we need this?
         */
        void resetId(void);

        /**
         * \brief Getter for error code.
         * \return Error code
         * \sa Cods
         */
        int getErrorCode(void) const;

        /**
         * \brief Setter of error code.
         * \param code Error code
         * \sa Cods.
         */
        void setErrorCode(int code);

        /**
         * \brief Getter of error string
         * \return Error string
         */
        const std::string& getErrorString(void) const;

        /**
         * \brief Setter of error string
         * \param message Error string.
         */
        void setErrorString(const std::string& message);

    private:
    /**
      *\brief Id of message
      * //! setting mId to 0 will be result of "resetting", in such packets id passed as null (error only)
    */
	int mId;

        /**
          *\brief Error code
        */
        int mCode;

        /**
          *\brief Error string
        */
        std::string mMessage;
        
        friend class RPC2ErrorMarshaller;
    };
}

#endif  //  RPC2COMMUNICATION_RPC2ERROR
