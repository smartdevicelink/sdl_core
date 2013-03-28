/**
* \file SDLRPCResponse.cpp
* \brief SDLRPCResponse class source file.
* Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
*/


#include "JSONHandler/SDLRPCResponse.h"
//#include "../src/JSONHandler/ALRPCObjectsImpl/V1/ResultMarshaller.h"

using namespace NsSmartDeviceLinkRPC;

SDLRPCResponse::SDLRPCResponse(unsigned int protocolVersion)
    : SDLRPCMessage(protocolVersion, RESPONSE)
    , mCorrelationID(0)
    , info(NULL)
    , success(false)
    , resultCode(NsSmartDeviceLinkRPCV2::Result::INVALID_ENUM)
{}

SDLRPCResponse::SDLRPCResponse(void)
    : SDLRPCMessage(0, RESPONSE)
    , mCorrelationID(0)
    , info(NULL)
    , success(false)
    , resultCode(NsSmartDeviceLinkRPCV2::Result::INVALID_ENUM)
{}

SDLRPCResponse::SDLRPCResponse(unsigned int protocolVersion,  int methodId)
    : SDLRPCMessage(protocolVersion, RESPONSE, methodId)
    , mCorrelationID(0)
    , info(NULL)
    , success(false)
    , resultCode(NsSmartDeviceLinkRPCV2::Result::INVALID_ENUM)
{}

SDLRPCResponse::~SDLRPCResponse()
{
    if (info)
    {
        delete info;
    }
}

unsigned int SDLRPCResponse::getCorrelationID() const
{
    return mCorrelationID;
}

void SDLRPCResponse::setCorrelationID(unsigned int correlationID)
{
    mCorrelationID = correlationID;
}

bool SDLRPCResponse::get_success(void) const
{
    return success;
}

const NsSmartDeviceLinkRPCV2::Result& SDLRPCResponse::get_resultCode(void) const
{
    return resultCode;
}

bool SDLRPCResponse::set_success(bool success_)
{
    success = success_;
    return true;
}

bool SDLRPCResponse::set_resultCode(const NsSmartDeviceLinkRPCV2::Result& resultCode_)
{
    /*if (!NsSmartDeviceLinkRPC::ResultMarshaller::checkIntegrityConst(resultCode_))
    {
        return false;
    }*/

    resultCode = resultCode_;
    return true;
}

bool SDLRPCResponse::set_info(const std::string& info_)
{
    if (info_.length() > 1000)
    {
        return false;
    }

    delete info;
    info = NULL;

    info = new std::string(info_);
    return true;
}

void SDLRPCResponse::reset_info(void)
{
    if (info)
    {
        delete info;
    }

    info = NULL;
}

const std::string* SDLRPCResponse::get_info(void) const
{
    return info;
}
