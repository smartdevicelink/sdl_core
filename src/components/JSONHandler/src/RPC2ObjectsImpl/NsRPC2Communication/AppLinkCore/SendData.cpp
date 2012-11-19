#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/SendData.h"
#include "../../../../../Marshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Mon Nov 19 07:25:04 2012
  source stamp	Mon Nov 19 07:04:40 2012
  author	robok0der
*/

using namespace NsRPC2Communication::AppLinkCore;


SendData& SendData::operator =(const SendData& c)
{
  data=c.data;
  return *this;
}


SendData::~SendData(void)
{
}


SendData::SendData(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__SENDDATA)
{
}


SendData::SendData(const SendData& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__SENDDATA,c.getId())
{
  *this=c;
}


const std::vector< std::string>& SendData::get_data(void)
{
  return data;
}

bool SendData::set_data(const std::vector< std::string>& data_)
{
  data=data_;
  return true;
}

bool SendData::checkIntegrity(void)
{
  return SendDataMarshaller::checkIntegrity(*this);
}
