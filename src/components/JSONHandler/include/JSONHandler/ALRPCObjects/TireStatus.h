#ifndef NSAPPLINKRPC_TIRESTATUS_INCLUDE
#define NSAPPLINKRPC_TIRESTATUS_INCLUDE


#include "SingleTireStatus.h"
#include "SingleTireStatus.h"
#include "SingleTireStatus.h"
#include "SingleTireStatus.h"
#include "TirePressureTellTale.h"
#include "SingleTireStatus.h"
#include "SingleTireStatus.h"


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
