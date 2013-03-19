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

#include "../include/JSONHandler/SDLRPCObjects/V2/VehicleType.h"
#include "VehicleTypeMarshaller.h"

/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/



using namespace NsSmartDeviceLinkRPCV2;

VehicleType& VehicleType::operator =(const VehicleType& c)
{
  make=c.make ? new std::string(c.make[0]) : 0;
  model=c.model ? new std::string(c.model[0]) : 0;
  modelYear=c.modelYear ? new std::string(c.modelYear[0]) : 0;
  trim=c.trim ? new std::string(c.trim[0]) : 0;

  return *this;
}


VehicleType::~VehicleType(void)
{
  if(make)
    delete make;
  if(model)
    delete model;
  if(modelYear)
    delete modelYear;
  if(trim)
    delete trim;
}


VehicleType::VehicleType(const VehicleType& c)
{
  *this=c;
}


bool VehicleType::checkIntegrity(void)
{
  return VehicleTypeMarshaller::checkIntegrity(*this);
}


VehicleType::VehicleType(void) :
    make(0),
    model(0),
    modelYear(0),
    trim(0)
{
}



bool VehicleType::set_make(const std::string& make_)
{
  if(make_.length()>500)  return false;
  delete make;
  make=0;

  make=new std::string(make_);
  return true;
}

void VehicleType::reset_make(void)
{
  if(make)
    delete make;
  make=0;
}

bool VehicleType::set_model(const std::string& model_)
{
  if(model_.length()>500)  return false;
  delete model;
  model=0;

  model=new std::string(model_);
  return true;
}

void VehicleType::reset_model(void)
{
  if(model)
    delete model;
  model=0;
}

bool VehicleType::set_modelYear(const std::string& modelYear_)
{
  if(modelYear_.length()>500)  return false;
  delete modelYear;
  modelYear=0;

  modelYear=new std::string(modelYear_);
  return true;
}

void VehicleType::reset_modelYear(void)
{
  if(modelYear)
    delete modelYear;
  modelYear=0;
}

bool VehicleType::set_trim(const std::string& trim_)
{
  if(trim_.length()>500)  return false;
  delete trim;
  trim=0;

  trim=new std::string(trim_);
  return true;
}

void VehicleType::reset_trim(void)
{
  if(trim)
    delete trim;
  trim=0;
}




const std::string* VehicleType::get_make(void) const 
{
  return make;
}


const std::string* VehicleType::get_model(void) const 
{
  return model;
}


const std::string* VehicleType::get_modelYear(void) const 
{
  return modelYear;
}


const std::string* VehicleType::get_trim(void) const 
{
  return trim;
}


