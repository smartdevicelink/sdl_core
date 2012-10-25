#ifndef TIRESTATUS_INCLUDE
#define TIRESTATUS_INCLUDE


#include "SingleTireStatus.h"
#include "SingleTireStatus.h"
#include "SingleTireStatus.h"
#include "SingleTireStatus.h"
#include "TirePressureTellTale.h"
#include "SingleTireStatus.h"
#include "SingleTireStatus.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/



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

  SingleTireStatus innerLeftRear;
  SingleTireStatus innerRightRear;
  SingleTireStatus leftFront;
  SingleTireStatus leftRear;
  TirePressureTellTale pressureTellTale;
  SingleTireStatus rightFront;
  SingleTireStatus rightRear;
};

#endif
