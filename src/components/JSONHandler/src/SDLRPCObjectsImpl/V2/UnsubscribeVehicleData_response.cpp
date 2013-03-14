#include "../include/JSONHandler/SDLRPCObjects/V2/UnsubscribeVehicleData_response.h"
#include "UnsubscribeVehicleData_responseMarshaller.h"
#include "VehicleDataResultMarshaller.h"

namespace
{
    const int PROTOCOL_VERSION = 2;
}

/*
  interface Ford Sync RAPI
  version   2.0O
  date      2012-11-02
  generated at  Thu Jan 24 06:36:23 2013
  source stamp  Thu Jan 24 06:35:41 2013
  author    robok0der
*/

using namespace NsSmartDeviceLinkRPCV2;
UnsubscribeVehicleData_response& UnsubscribeVehicleData_response::operator =(const UnsubscribeVehicleData_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;
    dataResult = c.dataResult ? new std::vector<VehicleDataResult>(c.dataResult[0]) : 0;

    return *this;
}

UnsubscribeVehicleData_response::~UnsubscribeVehicleData_response(void)
{
    if (dataResult)
    {
        delete dataResult;
    }
}

UnsubscribeVehicleData_response::UnsubscribeVehicleData_response(const UnsubscribeVehicleData_response& c)
{
    *this = c;
}

bool UnsubscribeVehicleData_response::checkIntegrity(void)
{
    return UnsubscribeVehicleData_responseMarshaller::checkIntegrity(*this);
}

UnsubscribeVehicleData_response::UnsubscribeVehicleData_response(void)
    : NsSmartDeviceLinkRPC::SDLRPCResponse(PROTOCOL_VERSION)
    , dataResult(0)
{}

bool UnsubscribeVehicleData_response::set_dataResult(const std::vector<VehicleDataResult>& dataResult_)
{
    unsigned int i = dataResult_.size();
    if (i > 100 || i < 1)
    {
        return false;
    }
    while (i--)
    {
        if (!VehicleDataResultMarshaller::checkIntegrityConst(dataResult_[i]))
        {
            return false;
        }
    }
    delete dataResult;
    dataResult = 0;

    dataResult = new std::vector<VehicleDataResult>(dataResult_);
    return true;
}

void UnsubscribeVehicleData_response::reset_dataResult(void)
{
    if (dataResult)
    {
        delete dataResult;
    }
    dataResult = 0;
}

const std::vector<VehicleDataResult>* UnsubscribeVehicleData_response::get_dataResult(void) const
{
    return dataResult;
}

