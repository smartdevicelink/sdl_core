#include "../include/JSONHandler/ALRPCObjects/V2/DTC.h"
#include "DTCMarshaller.h"

/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/



using namespace NsAppLinkRPCV2;

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


