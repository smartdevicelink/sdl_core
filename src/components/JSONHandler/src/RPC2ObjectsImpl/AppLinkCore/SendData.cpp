#include "../include/JSONHandler/RPC2Objects/AppLinkCore/SendData.h"
#include "../include/JSONHandler/RPC2Objects/AppLinkCore/Marshaller.h"

/*
  interface	RPC2Communication::UI
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace AppLinkRPC;
using namespace RPC2Communication::AppLinkCore;


SendData& SendData::operator =(const SendData& c)
{
  data=c.data;
  return *this;
}


SendData::~SendData(void)
{
}


SendData::SendData(void) :
  RPC2Request(Marshaller::METHOD_SENDDATA)
{
}


SendData::SendData(const SendData& c) : RPC2Request(Marshaller::METHOD_SENDDATA,c.getId())
{
  *this=c;
}

const std::vector< std::string >& SendData::get_data(void)
{
  return data;
}

bool SendData::set_data(const std::vector< std::string >& data_)
{
  data=data_;
  return true;
}

bool SendData::checkIntegrity(void)
{
  return SendDataMarshaller::checkIntegrity(*this);
}

