#include "../../include/JSONHandler/ALRPCObjects/VehicleType.h"
#include "VehicleTypeMarshaller.h"

/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/



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


