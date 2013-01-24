#ifndef NSAPPLINKRPCV2_VEHICLETYPE_INCLUDE
#define NSAPPLINKRPCV2_VEHICLETYPE_INCLUDE

#include <string>



/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/


namespace NsAppLinkRPCV2
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
