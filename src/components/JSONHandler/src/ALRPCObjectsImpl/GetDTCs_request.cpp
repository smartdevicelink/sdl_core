#include "../../include/JSONHandler/ALRPCObjects/GetDTCs_request.h"
#include "GetDTCs_requestMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/

GetDTCs_request& GetDTCs_request::operator =(const GetDTCs_request& c)
{
  ecuName= c.ecuName ? new unsigned int(c.ecuName[0]) : 0;
  encrypted= c.encrypted ? new bool(c.encrypted[0]) : 0;

  return *this;}


GetDTCs_request::~GetDTCs_request(void)
{
  if(ecuName)
    delete ecuName;
  if(encrypted)
    delete encrypted;
}


GetDTCs_request::GetDTCs_request(const GetDTCs_request& c)
{
  *this=c;
}


bool GetDTCs_request::checkIntegrity(void)
{
  return GetDTCs_requestMarshaller::checkIntegrity(*this);
}


GetDTCs_request::GetDTCs_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_GETDTCS_REQUEST),
      ecuName(0),
    encrypted(0)
{
}



bool GetDTCs_request::set_ecuName(unsigned int ecuName_)
{
  if(ecuName_>65535)  return false;
  delete ecuName;
  ecuName=0;

  ecuName=new unsigned int(ecuName_);
  return true;
}

void GetDTCs_request::reset_ecuName(void)
{
  if(ecuName)
    delete ecuName;
  ecuName=0;
}

bool GetDTCs_request::set_encrypted(bool encrypted_)
{
  delete encrypted;
  encrypted=0;

  encrypted=new bool(encrypted_);
  return true;
}

void GetDTCs_request::reset_encrypted(void)
{
  if(encrypted)
    delete encrypted;
  encrypted=0;
}




const unsigned int* GetDTCs_request::get_ecuName(void) const 
{
  return ecuName;
}

const bool* GetDTCs_request::get_encrypted(void) const 
{
  return encrypted;
}

