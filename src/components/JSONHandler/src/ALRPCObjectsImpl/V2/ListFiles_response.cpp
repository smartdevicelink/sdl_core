#include "../include/JSONHandler/ALRPCObjects/V2/ListFiles_response.h"
#include "ListFiles_responseMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "ResultMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsAppLinkRPCV2;
ListFiles_response& ListFiles_response::operator =(const ListFiles_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  filenames= c.filenames ? new std::vector<std::string>(c.filenames[0]) : 0;
  spaceAvailable= c.spaceAvailable;
  info= c.info ? new std::string(c.info[0]) : 0;

  return *this;
}


ListFiles_response::~ListFiles_response(void)
{
  if(filenames)
    delete filenames;
  if(info)
    delete info;
}


ListFiles_response::ListFiles_response(const ListFiles_response& c) : NsAppLinkRPC::ALRPCMessage(c)
{
  *this=c;
}


bool ListFiles_response::checkIntegrity(void)
{
  return ListFiles_responseMarshaller::checkIntegrity(*this);
}


ListFiles_response::ListFiles_response(void) : NsAppLinkRPC::ALRPCMessage(PROTOCOL_VERSION),
      filenames(0),
    info(0)
{
}



bool ListFiles_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool ListFiles_response::set_resultCode(const Result& resultCode_)
{
  if(!ResultMarshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool ListFiles_response::set_filenames(const std::vector<std::string>& filenames_)
{
  unsigned int i=filenames_.size();
  if(i>1000 || i<0)  return false;
  while(i--)
  {
    if(filenames_[i].length()>500)  return false;
  }
  delete filenames;
  filenames=0;

  filenames=new std::vector<std::string>(filenames_);
  return true;
}

void ListFiles_response::reset_filenames(void)
{
  if(filenames)
    delete filenames;
  filenames=0;
}

bool ListFiles_response::set_spaceAvailable(unsigned int spaceAvailable_)
{
  if(spaceAvailable_>2000000000)  return false;
  spaceAvailable=spaceAvailable_;
  return true;
}

bool ListFiles_response::set_info(const std::string& info_)
{
  if(info_.length()>1000)  return false;
  delete info;
  info=0;

  info=new std::string(info_);
  return true;
}

void ListFiles_response::reset_info(void)
{
  if(info)
    delete info;
  info=0;
}




bool ListFiles_response::get_success(void) const
{
  return success;
}

const Result& ListFiles_response::get_resultCode(void) const 
{
  return resultCode;
}

const std::vector<std::string>* ListFiles_response::get_filenames(void) const 
{
  return filenames;
}

unsigned int ListFiles_response::get_spaceAvailable(void) const
{
  return spaceAvailable;
}

const std::string* ListFiles_response::get_info(void) const 
{
  return info;
}

