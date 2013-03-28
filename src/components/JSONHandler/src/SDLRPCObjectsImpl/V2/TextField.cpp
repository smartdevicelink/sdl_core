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

#include "../include/JSONHandler/SDLRPCObjects/V2/TextField.h"
#include "TextFieldMarshaller.h"
#include "CharacterSetMarshaller.h"
#include "TextFieldNameMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/



using namespace NsSmartDeviceLinkRPCV2;

TextField::TextField(const TextField& c)
{
  *this=c;
}


bool TextField::checkIntegrity(void)
{
  return TextFieldMarshaller::checkIntegrity(*this);
}


TextField::TextField(void)
{
}



bool TextField::set_characterSet(const CharacterSet& characterSet_)
{
  if(!CharacterSetMarshaller::checkIntegrityConst(characterSet_))   return false;
  characterSet=characterSet_;
  return true;
}

bool TextField::set_name(const TextFieldName& name_)
{
  if(!TextFieldNameMarshaller::checkIntegrityConst(name_))   return false;
  name=name_;
  return true;
}

bool TextField::set_rows(unsigned int rows_)
{
  if(rows_>3)  return false;
  if(rows_<1)  return false;
  rows=rows_;
  return true;
}

bool TextField::set_width(unsigned int width_)
{
  if(width_>500)  return false;
  if(width_<1)  return false;
  width=width_;
  return true;
}




const CharacterSet& TextField::get_characterSet(void) const 
{
  return characterSet;
}


const TextFieldName& TextField::get_name(void) const 
{
  return name;
}


unsigned int TextField::get_rows(void) const
{
  return rows;
}


unsigned int TextField::get_width(void) const
{
  return width;
}


