/**
* \file ALRPCResponse.cpp
* \brief ALRPCResponse class source file.
* \author PVyshnevska
*/


#include "JSONHandler/ALRPCResponse.h"
//#include "../src/JSONHandler/ALRPCObjectsImpl/V1/ResultMarshaller.h"

using namespace NsAppLinkRPC;

ALRPCResponse::ALRPCResponse(unsigned int protocolVersion)
    : ALRPCMessage(protocolVersion, RESPONSE)
    , mCorrelationID(0)
    , info(NULL)
    , success(false)
    , resultCode(NsAppLinkRPCV2::Result::INVALID_ENUM)
{}

ALRPCResponse::ALRPCResponse(void)
    : ALRPCMessage(0, RESPONSE)
    , mCorrelationID(0)
    , info(NULL)
    , success(false)
    , resultCode(NsAppLinkRPCV2::Result::INVALID_ENUM)
{}

ALRPCResponse::ALRPCResponse(unsigned int protocolVersion,  int methodId)
    : ALRPCMessage(protocolVersion, RESPONSE, methodId)
    , mCorrelationID(0)
    , info(NULL)
    , success(false)
    , resultCode(NsAppLinkRPCV2::Result::INVALID_ENUM)
{}

ALRPCResponse::~ALRPCResponse()
{
    if (info)
    {
        delete info;
    }
}

unsigned int ALRPCResponse::getCorrelationID() const
{
    return mCorrelationID;
}

void ALRPCResponse::setCorrelationID(unsigned int correlationID)
{
    mCorrelationID = correlationID;
}

bool ALRPCResponse::get_success(void) const
{
    return success;
}

const NsAppLinkRPCV2::Result& ALRPCResponse::get_resultCode(void) const
{
    return resultCode;
}

bool ALRPCResponse::set_success(bool success_)
{
    success = success_;
    return true;
}

bool ALRPCResponse::set_resultCode(const NsAppLinkRPCV2::Result& resultCode_)
{
    /*if (!NsAppLinkRPC::ResultMarshaller::checkIntegrityConst(resultCode_))
    {
        return false;
    }*/

    resultCode = resultCode_;
    return true;
}

bool ALRPCResponse::set_info(const std::string& info_)
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

void ALRPCResponse::reset_info(void)
{
    if (info)
    {
        delete info;
    }

    info = NULL;
}

const std::string* ALRPCResponse::get_info(void) const
{
    return info;
}
