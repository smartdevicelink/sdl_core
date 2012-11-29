#include "../include/JSONHandler/ALRPCObjects/TextField_v2.h"
#include "TextField_v2Marshaller.h"
#include "CharacterSetMarshaller.h"
#include "TextFieldName_v2Marshaller.h"
/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/



using namespace NsAppLinkRPC;

TextField_v2::TextField_v2(const TextField_v2& c)
{
  *this=c;
}


bool TextField_v2::checkIntegrity(void)
{
  return TextField_v2Marshaller::checkIntegrity(*this);
}


TextField_v2::TextField_v2(void)
{
}



bool TextField_v2::set_characterSet(const CharacterSet& characterSet_)
{
  if(!CharacterSetMarshaller::checkIntegrityConst(characterSet_))   return false;
  characterSet=characterSet_;
  return true;
}

bool TextField_v2::set_name(const TextFieldName_v2& name_)
{
  if(!TextFieldName_v2Marshaller::checkIntegrityConst(name_))   return false;
  name=name_;
  return true;
}

bool TextField_v2::set_rows(unsigned int rows_)
{
  if(rows_>3)  return false;
  if(rows_<1)  return false;
  rows=rows_;
  return true;
}

bool TextField_v2::set_width(unsigned int width_)
{
  if(width_>500)  return false;
  if(width_<1)  return false;
  width=width_;
  return true;
}




const CharacterSet& TextField_v2::get_characterSet(void) const 
{
  return characterSet;
}


const TextFieldName_v2& TextField_v2::get_name(void) const 
{
  return name;
}


unsigned int TextField_v2::get_rows(void) const
{
  return rows;
}


unsigned int TextField_v2::get_width(void) const
{
  return width;
}


