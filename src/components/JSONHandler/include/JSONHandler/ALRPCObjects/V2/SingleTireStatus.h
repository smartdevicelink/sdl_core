#ifndef NSAPPLINKRPCV2_SINGLETIRESTATUS_INCLUDE
#define NSAPPLINKRPCV2_SINGLETIRESTATUS_INCLUDE


#include "SingleTirePressureStatus.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/


namespace NsAppLinkRPCV2
{

  class SingleTireStatus
  {
  public:
  
    SingleTireStatus(const SingleTireStatus& c);
    SingleTireStatus(void);
  
    bool checkIntegrity(void);
  // getters

    unsigned int get_pressure(void) const;
    const SingleTirePressureStatus& get_status(void) const;

// setters

    bool set_pressure(unsigned int pressure_);
    bool set_status(const SingleTirePressureStatus& status_);

  private:

    friend class SingleTireStatusMarshaller;


///  The tire pressure in PSI.
      unsigned int pressure;	//!<  (0,255)

///  See SingleTirePressureStatus.
      SingleTirePressureStatus status;
  };

}

#endif
