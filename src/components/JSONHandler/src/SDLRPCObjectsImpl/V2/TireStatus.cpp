//
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#include "../include/JSONHandler/SDLRPCObjects/V2/TireStatus.h"
#include "TireStatusMarshaller.h"
#include "SingleTireStatusMarshaller.h"
#include "TirePressureTellTaleMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/



using namespace NsSmartDeviceLinkRPCV2;

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


