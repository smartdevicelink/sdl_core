#include "../../include/JSONHandler/ALRPCObjects/PubSubData.h"
#include "PubSubDataMarshaller.h"
#include "PublishedDataMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/



PubSubData::PubSubData(const PubSubData& c)
{
  *this=c;
}


bool PubSubData::checkIntegrity(void)
{
  return PubSubDataMarshaller::checkIntegrity(*this);
}


PubSubData::PubSubData(void)
{
}



bool PubSubData::set_data(const PublishedData& data_)
{
  if(!PublishedDataMarshaller::checkIntegrityConst(data_))   return false;
  data=data_;
  return true;
}




const PublishedData& PubSubData::get_data(void) const 
{
  return data;
}


