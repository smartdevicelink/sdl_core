#include "../../include/JSONHandler/ALRPCObjects/DTC.h"
#include "DTCMarshaller.h"

/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/



DTC::DTC(const DTC& c)
{
  *this=c;
}


bool DTC::checkIntegrity(void)
{
  return DTCMarshaller::checkIntegrity(*this);
}


DTC::DTC(void)
{
}



bool DTC::set_identifier(const std::string& identifier_)
{
  if(identifier_.length()>500)  return false;
  identifier=identifier_;
  return true;
}

bool DTC::set_statusByte(const std::string& statusByte_)
{
  if(statusByte_.length()>500)  return false;
  statusByte=statusByte_;
  return true;
}




const std::string& DTC::get_identifier(void) const 
{
  return identifier;
}


const std::string& DTC::get_statusByte(void) const 
{
  return statusByte;
}


