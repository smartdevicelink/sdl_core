#include "../include/JSONHandler/ALRPCObjects/V2/ReadDID_response.h"
#include "ReadDID_responseMarshaller.h"
#include "VehicleDataResultCodeMarshaller.h"

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

using namespace NsAppLinkRPCV2;
ReadDID_response& ReadDID_response::operator =(const ReadDID_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;
    dataResult = c.dataResult ? new std::vector<VehicleDataResultCode>(c.dataResult[0]) : 0;
    data = c.data ? new std::vector<std::string>(c.data[0]) : 0;

    return *this;
}

ReadDID_response::~ReadDID_response(void)
{
    if (dataResult)
    {
        delete dataResult;
    }
    if (data)
    {
        delete data;
    }
}

ReadDID_response::ReadDID_response(const ReadDID_response& c)
{
    *this = c;
}

bool ReadDID_response::checkIntegrity(void)
{
    return ReadDID_responseMarshaller::checkIntegrity(*this);
}

ReadDID_response::ReadDID_response(void)
    : NsAppLinkRPC::ALRPCResponse(PROTOCOL_VERSION)
    , dataResult(0)
    , data(0)
{
}

bool ReadDID_response::set_dataResult(const std::vector<VehicleDataResultCode>& dataResult_)
{
    unsigned int i = dataResult_.size();
    if (i > 1000 || i < 0)  // TODO(akandul): unsigned int i < 0
    {
        return false;
    }
    while (i--)
    {
        if (!VehicleDataResultCodeMarshaller::checkIntegrityConst(dataResult_[i]))
        {
            return false;
        }
    }
    delete dataResult;
    dataResult = 0;

    dataResult = new std::vector<VehicleDataResultCode>(dataResult_);
    return true;
}

void ReadDID_response::reset_dataResult(void)
{
    if (dataResult)
    {
        delete dataResult;
    }
    dataResult = 0;
}

bool ReadDID_response::set_data(const std::vector<std::string>& data_)
{
    unsigned int i = data_.size();
    if (i > 1000 || i < 0)  // TODO(akandul): unsigned int i < 0
    {
        return false;
    }
    while (i--)
    {
        if (data_[i].length() > 5000)
        {
            return false;
        }
    }
    delete data;
    data = 0;

    data = new std::vector<std::string>(data_);
    return true;
}

void ReadDID_response::reset_data(void)
{
    if (data)
    {
        delete data;
    }
    data = 0;
}

const std::vector<VehicleDataResultCode>* ReadDID_response::get_dataResult(void) const
{
    return dataResult;
}

const std::vector<std::string>* ReadDID_response::get_data(void) const
{
    return data;
}

