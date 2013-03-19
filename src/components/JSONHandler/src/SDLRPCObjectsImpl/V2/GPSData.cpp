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

#include "../include/JSONHandler/SDLRPCObjects/V2/GPSData.h"
#include "GPSDataMarshaller.h"
#include "CompassDirectionMarshaller.h"
#include "DimensionMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/



using namespace NsSmartDeviceLinkRPCV2;

GPSData::GPSData(const GPSData& c)
{
  *this=c;
}


bool GPSData::checkIntegrity(void)
{
  return GPSDataMarshaller::checkIntegrity(*this);
}


GPSData::GPSData(void)
{
}



bool GPSData::set_actual(bool actual_)
{
  actual=actual_;
  return true;
}

bool GPSData::set_altitude(int altitude_)
{
  if(altitude_>10000)  return false;
  altitude=altitude_;
  return true;
}

bool GPSData::set_compassDirection(const CompassDirection& compassDirection_)
{
  if(!CompassDirectionMarshaller::checkIntegrityConst(compassDirection_))   return false;
  compassDirection=compassDirection_;
  return true;
}

bool GPSData::set_dimension(const Dimension& dimension_)
{
  if(!DimensionMarshaller::checkIntegrityConst(dimension_))   return false;
  dimension=dimension_;
  return true;
}

bool GPSData::set_hdop(unsigned int hdop_)
{
  if(hdop_>31)  return false;
  hdop=hdop_;
  return true;
}

bool GPSData::set_heading(unsigned int heading_)
{
  if(heading_>360)  return false;
  heading=heading_;
  return true;
}

bool GPSData::set_latitudeDegrees(int latitudeDegrees_)
{
  if(latitudeDegrees_>1000000000)  return false;
  latitudeDegrees=latitudeDegrees_;
  return true;
}

bool GPSData::set_longitudeDegrees(int longitudeDegrees_)
{
  if(longitudeDegrees_>1000000000)  return false;
  longitudeDegrees=longitudeDegrees_;
  return true;
}

bool GPSData::set_pdop(unsigned int pdop_)
{
  if(pdop_>31)  return false;
  pdop=pdop_;
  return true;
}

bool GPSData::set_satellites(unsigned int satellites_)
{
  if(satellites_>31)  return false;
  satellites=satellites_;
  return true;
}

bool GPSData::set_speed(unsigned int speed_)
{
  if(speed_>400)  return false;
  speed=speed_;
  return true;
}

bool GPSData::set_utcDay(unsigned int utcDay_)
{
  if(utcDay_>31)  return false;
  if(utcDay_<1)  return false;
  utcDay=utcDay_;
  return true;
}

bool GPSData::set_utcHours(unsigned int utcHours_)
{
  if(utcHours_>23)  return false;
  utcHours=utcHours_;
  return true;
}

bool GPSData::set_utcMinutes(unsigned int utcMinutes_)
{
  if(utcMinutes_>59)  return false;
  utcMinutes=utcMinutes_;
  return true;
}

bool GPSData::set_utcMonth(unsigned int utcMonth_)
{
  if(utcMonth_>12)  return false;
  if(utcMonth_<1)  return false;
  utcMonth=utcMonth_;
  return true;
}

bool GPSData::set_utcSeconds(unsigned int utcSeconds_)
{
  if(utcSeconds_>59)  return false;
  utcSeconds=utcSeconds_;
  return true;
}

bool GPSData::set_utcYear(unsigned int utcYear_)
{
  if(utcYear_>2100)  return false;
  if(utcYear_<2010)  return false;
  utcYear=utcYear_;
  return true;
}

bool GPSData::set_vdop(unsigned int vdop_)
{
  if(vdop_>31)  return false;
  vdop=vdop_;
  return true;
}




bool GPSData::get_actual(void) const
{
  return actual;
}


int GPSData::get_altitude(void) const
{
  return altitude;
}


const CompassDirection& GPSData::get_compassDirection(void) const 
{
  return compassDirection;
}


const Dimension& GPSData::get_dimension(void) const 
{
  return dimension;
}


unsigned int GPSData::get_hdop(void) const
{
  return hdop;
}


unsigned int GPSData::get_heading(void) const
{
  return heading;
}


int GPSData::get_latitudeDegrees(void) const
{
  return latitudeDegrees;
}


int GPSData::get_longitudeDegrees(void) const
{
  return longitudeDegrees;
}


unsigned int GPSData::get_pdop(void) const
{
  return pdop;
}


unsigned int GPSData::get_satellites(void) const
{
  return satellites;
}


unsigned int GPSData::get_speed(void) const
{
  return speed;
}


unsigned int GPSData::get_utcDay(void) const
{
  return utcDay;
}


unsigned int GPSData::get_utcHours(void) const
{
  return utcHours;
}


unsigned int GPSData::get_utcMinutes(void) const
{
  return utcMinutes;
}


unsigned int GPSData::get_utcMonth(void) const
{
  return utcMonth;
}


unsigned int GPSData::get_utcSeconds(void) const
{
  return utcSeconds;
}


unsigned int GPSData::get_utcYear(void) const
{
  return utcYear;
}


unsigned int GPSData::get_vdop(void) const
{
  return vdop;
}


