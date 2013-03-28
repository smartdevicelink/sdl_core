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

#ifndef NSSMARTDEVICELINKKRPCV2_VEHICLETYPE_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_VEHICLETYPE_INCLUDE

#include <string>



/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/


namespace NsSmartDeviceLinkRPCV2
{

  class VehicleType
  {
  public:
  
    VehicleType(const VehicleType& c);
    VehicleType(void);
  
    bool checkIntegrity(void);
  
    ~VehicleType(void);
    VehicleType& operator =(const VehicleType&);

// getters

    const std::string* get_make(void) const;
    const std::string* get_model(void) const;
    const std::string* get_modelYear(void) const;
    const std::string* get_trim(void) const;

// setters

    void reset_make(void);
    bool set_make(const std::string& make_);
    void reset_model(void);
    bool set_model(const std::string& model_);
    void reset_modelYear(void);
    bool set_modelYear(const std::string& modelYear_);
    void reset_trim(void);
    bool set_trim(const std::string& trim_);

  private:

    friend class VehicleTypeMarshaller;


/**
     Make of the vehicle
     e.g. Ford
*/
      std::string* make;	//!< (500)

/**
     Model of the vehicle
     e.g. Fiesta
*/
      std::string* model;	//!< (500)

/**
     Model Year of the vehicle
     e.g. 2013
*/
      std::string* modelYear;	//!< (500)

/**
     Trim of the vehicle
     e.g. SE
*/
      std::string* trim;	//!< (500)
  };

}

#endif
