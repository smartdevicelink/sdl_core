/**
* \file RPC2Response.hpp
* \brief RPC2Response class header.
* \author PVyshnevska
*/


#ifndef RPC2_RESPONSE_CLASS
#define RPC2_RESPONSE_CLASS

#include "RPC2Command.h"

/**
  *\namespace RPC2Communication
  *\brief Namespace for communication with HMI.
*/
namespace RPC2Communication
{
    /**
     * \class RPC2Response
     * \brief Base class for responses generated from Json RPC2
    */
    class RPC2Response : public RPC2Command
    {
    public:
        /**
         * \brief Default Constructor
        */
        RPC2Response( );

        /**
         * \brief Constructor
         * \param method Method Id.
        */
        RPC2Response(int method);

        /**
         * \brief Constructor
         * \param method Method Id.
         * \param id Message id.
        */
        RPC2Response(int method ,unsigned int id);

        /**
         * \brief Constructor
         * \param method Method Id.
         * \param id Message id.
         * \param res Result code.
        */
        RPC2Response(int method ,unsigned int id,int res);

        /**
         * \brief Destructor
        */
        virtual ~RPC2Response();

        /**
         * \brief Getter of Message id.
         * \return Message id.
         */
        virtual unsigned int getId() const;

        /**
         * \brief Setter of Message id.
         * \param id Message id.
         */
        virtual void setId(unsigned int id);

        /**
         * \brief Getter of Result.
         * \return Result code.
         */
        virtual int getResult() const;

        /**
         * \brief Setter of Result.
         * \param r Result code.
         */
        virtual void setResult(int r);


    private:
        /**
          *\brief Message id
        */
        unsigned int mId;

        /**
          *\brief Result code
        */
        int mResultCode;

    };
}

#endif // RPC2_RESPONSE_CLASS