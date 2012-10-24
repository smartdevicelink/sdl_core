#ifndef SINGLETIRESTATUS_INCLUDE
#define SINGLETIRESTATUS_INCLUDE


#include "SingleTirePressureStatus.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/



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

  unsigned int pressure;	//!<  (0,255)
  SingleTirePressureStatus status;
};

#endif
