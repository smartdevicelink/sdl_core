#include "../include/JSONHandler/ALRPCObjects/V2/TireStatus.h"
#include "TireStatusMarshaller.h"
#include "SingleTireStatusMarshaller.h"
#include "TirePressureTellTaleMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/



using namespace NsAppLinkRPCV2;

TireStatus::TireStatus(const TireStatus& c)
{
  *this=c;
}


bool TireStatus::checkIntegrity(void)
{
  return TireStatusMarshaller::checkIntegrity(*this);
}


TireStatus::TireStatus(void)
{
}



bool TireStatus::set_innerLeftRear(const SingleTireStatus& innerLeftRear_)
{
  if(!SingleTireStatusMarshaller::checkIntegrityConst(innerLeftRear_))   return false;
  innerLeftRear=innerLeftRear_;
  return true;
}

bool TireStatus::set_innerRightRear(const SingleTireStatus& innerRightRear_)
{
  if(!SingleTireStatusMarshaller::checkIntegrityConst(innerRightRear_))   return false;
  innerRightRear=innerRightRear_;
  return true;
}

bool TireStatus::set_leftFront(const SingleTireStatus& leftFront_)
{
  if(!SingleTireStatusMarshaller::checkIntegrityConst(leftFront_))   return false;
  leftFront=leftFront_;
  return true;
}

bool TireStatus::set_leftRear(const SingleTireStatus& leftRear_)
{
  if(!SingleTireStatusMarshaller::checkIntegrityConst(leftRear_))   return false;
  leftRear=leftRear_;
  return true;
}

bool TireStatus::set_pressureTellTale(const TirePressureTellTale& pressureTellTale_)
{
  if(!TirePressureTellTaleMarshaller::checkIntegrityConst(pressureTellTale_))   return false;
  pressureTellTale=pressureTellTale_;
  return true;
}

bool TireStatus::set_rightFront(const SingleTireStatus& rightFront_)
{
  if(!SingleTireStatusMarshaller::checkIntegrityConst(rightFront_))   return false;
  rightFront=rightFront_;
  return true;
}

bool TireStatus::set_rightRear(const SingleTireStatus& rightRear_)
{
  if(!SingleTireStatusMarshaller::checkIntegrityConst(rightRear_))   return false;
  rightRear=rightRear_;
  return true;
}




const SingleTireStatus& TireStatus::get_innerLeftRear(void) const 
{
  return innerLeftRear;
}


const SingleTireStatus& TireStatus::get_innerRightRear(void) const 
{
  return innerRightRear;
}


const SingleTireStatus& TireStatus::get_leftFront(void) const 
{
  return leftFront;
}


const SingleTireStatus& TireStatus::get_leftRear(void) const 
{
  return leftRear;
}


const TirePressureTellTale& TireStatus::get_pressureTellTale(void) const 
{
  return pressureTellTale;
}


const SingleTireStatus& TireStatus::get_rightFront(void) const 
{
  return rightFront;
}


const SingleTireStatus& TireStatus::get_rightRear(void) const 
{
  return rightRear;
}


