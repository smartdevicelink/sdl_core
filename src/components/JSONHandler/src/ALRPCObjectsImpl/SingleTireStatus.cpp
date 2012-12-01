#include "../include/JSONHandler/ALRPCObjects/SingleTireStatus.h"
#include "SingleTireStatusMarshaller.h"
#include "SingleTirePressureStatusMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/



using namespace NsAppLinkRPC;

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


