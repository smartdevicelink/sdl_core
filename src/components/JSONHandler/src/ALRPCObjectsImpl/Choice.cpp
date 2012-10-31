#include "../../include/JSONHandler/ALRPCObjects/Choice.h"
#include "ChoiceMarshaller.h"

/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Oct 30 08:29:32 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/



Choice::Choice(const Choice& c)
{
  *this=c;
}


bool Choice::checkIntegrity(void)
{
  return ChoiceMarshaller::checkIntegrity(*this);
}


Choice::Choice(void)
{
}



bool Choice::set_choiceID(unsigned int choiceID_)
{
  if(choiceID_>65535)  return false;
  choiceID=choiceID_;
  return true;
}

bool Choice::set_menuName(const std::string& menuName_)
{
  if(menuName_.length()>500)  return false;
  menuName=menuName_;
  return true;
}

bool Choice::set_vrCommands(const std::vector<std::string>& vrCommands_)
{
  unsigned int i=vrCommands_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(vrCommands_[i].length()>99)  return false;
  }
  vrCommands=vrCommands_;
  return true;
}




unsigned int Choice::get_choiceID(void) const
{
  return choiceID;
}


const std::string& Choice::get_menuName(void) const 
{
  return menuName;
}


const std::vector<std::string>& Choice::get_vrCommands(void) const 
{
  return vrCommands;
}


