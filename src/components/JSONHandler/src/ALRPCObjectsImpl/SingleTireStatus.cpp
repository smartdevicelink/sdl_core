#include "../../include/JSONHandler/ALRPCObjects/SingleTireStatus.h"
#include "SingleTireStatusMarshaller.h"
#include "SingleTirePressureStatusMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/



SingleTireStatus::SingleTireStatus(const SingleTireStatus& c)
{
  *this=c;
}


bool SingleTireStatus::checkIntegrity(void)
{
  return SingleTireStatusMarshaller::checkIntegrity(*this);
}


SingleTireStatus::SingleTireStatus(void)
{
}



bool SingleTireStatus::set_pressure(unsigned int pressure_)
{
  if(pressure_>255)  return false;
  pressure=pressure_;
  return true;
}

bool SingleTireStatus::set_status(const SingleTirePressureStatus& status_)
{
  if(!SingleTirePressureStatusMarshaller::checkIntegrityConst(status_))   return false;
  status=status_;
  return true;
}




unsigned int SingleTireStatus::get_pressure(void) const
{
  return pressure;
}


const SingleTirePressureStatus& SingleTireStatus::get_status(void) const 
{
  return status;
}


