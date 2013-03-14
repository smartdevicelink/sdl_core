/**
* \file SDLRPCResponse.cpp
* \brief SDLRPCResponse class source file.
* \author PVyshnevska
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
    /*if (!NsAppLinkRPC::ResultMarshaller::checkIntegrityConst(resultCode_))
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
