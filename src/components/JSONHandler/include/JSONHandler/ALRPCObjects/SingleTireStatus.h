#ifndef NSAPPLINKRPC_SINGLETIRESTATUS_INCLUDE
#define NSAPPLINKRPC_SINGLETIRESTATUS_INCLUDE


#include "SingleTirePressureStatus.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/


namespace NsAppLinkRPC
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
