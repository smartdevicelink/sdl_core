/**
* \file RPC2Response.hpp
* \brief RPC2Response class header.
* Copyright (c) 2013, Ford Motor Company
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following
* disclaimer in the documentation and/or other materials provided with the
* distribution.
*
* Neither the name of the Ford Motor Company nor the names of its contributors
* may be used to endorse or promote products derived from this software
* without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef RPC2_RESPONSE_CLASS
#define RPC2_RESPONSE_CLASS

#include "RPC2Command.h"

/**
  *\namespace NsRPC2Communication
  *\brief Namespace for communication with HMI.
*/
namespace NsRPC2Communication
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
