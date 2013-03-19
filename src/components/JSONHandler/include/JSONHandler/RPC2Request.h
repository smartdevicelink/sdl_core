/**
* \file RPC2Request.hpp
* \brief RPC2Request class header.
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


#ifndef RPC2_REQUEST_CLASS
#define RPC2_REQUEST_CLASS

#include "RPC2Command.h"

/**
  *\namespace NsRPC2Communication
  *\brief Namespace for communication with HMI.
*/
namespace NsRPC2Communication
{  
    /**
     * \class RPC2Request
     * \brief Base class for requests generated from Json RPC2
    */
    class RPC2Request : public RPC2Command
    {
    public:
        /**
         * \brief Default Constructor
        */
        RPC2Request( );

        /**
         * \brief Constructor
         * \param method Method Id.
        */
        RPC2Request( int method );

        /**
         * \brief Constructor
         * \param method Method Id.
         * \param id Message id.
        */
        RPC2Request( int method ,unsigned int id);

        /**
         * \brief Destructor
        */
        virtual ~RPC2Request();

        /**
         * \brief Getter of message id.
         * \return Message id.
         */
        virtual unsigned int getId() const;

        /**
         * \brief Setter of message id.
         * \param id Message id.
         */
        virtual void setId(unsigned int id);        

    private:
        /**
          *\brief Message id
          * //! mId should be >0
        */
        unsigned int mId;
        
    };

}

#endif  //  RPC2_REQUEST_CLASS
