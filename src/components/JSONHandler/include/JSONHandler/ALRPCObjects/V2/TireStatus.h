#ifndef NSAPPLINKRPCV2_TIRESTATUS_INCLUDE
#define NSAPPLINKRPCV2_TIRESTATUS_INCLUDE


#include "SingleTireStatus.h"
#include "SingleTireStatus.h"
#include "SingleTireStatus.h"
#include "SingleTireStatus.h"
#include "TirePressureTellTale.h"
#include "SingleTireStatus.h"
#include "SingleTireStatus.h"


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

///  The status and pressure of the tires.

  class TireStatus
  {
  public:
  
    TireStatus(const TireStatus& c);
    TireStatus(void);
  
    bool checkIntegrity(void);
  // getters

    const SingleTireStatus& get_innerLeftRear(void) const;
    const SingleTireStatus& get_innerRightRear(void) const;
    const SingleTireStatus& get_leftFront(void) const;
    const SingleTireStatus& get_leftRear(void) const;
    const TirePressureTellTale& get_pressureTellTale(void) const;
    const SingleTireStatus& get_rightFront(void) const;
    const SingleTireStatus& get_rightRear(void) const;

// setters

    bool set_innerLeftRear(const SingleTireStatus& innerLeftRear_);
    bool set_innerRightRear(const SingleTireStatus& innerRightRear_);
    bool set_leftFront(const SingleTireStatus& leftFront_);
    bool set_leftRear(const SingleTireStatus& leftRear_);
    bool set_pressureTellTale(const TirePressureTellTale& pressureTellTale_);
    bool set_rightFront(const SingleTireStatus& rightFront_);
    bool set_rightRear(const SingleTireStatus& rightRear_);

  private:

    friend class TireStatusMarshaller;


///  The status of the inner left rear.
      SingleTireStatus innerLeftRear;

///  The status of the inner right rear.
      SingleTireStatus innerRightRear;

///  The status of the left front tire.
      SingleTireStatus leftFront;

///  The status of the left rear tire.
      SingleTireStatus leftRear;

///  See TirePressureTellTale.
      TirePressureTellTale pressureTellTale;

///  The status of the right front tire.
      SingleTireStatus rightFront;

///  The status of the right rear tire.
      SingleTireStatus rightRear;
  };

}

#endif
