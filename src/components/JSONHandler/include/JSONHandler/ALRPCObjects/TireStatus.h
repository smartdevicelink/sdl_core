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
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
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
