#ifndef SINGLETIRESTATUS_INCLUDE
#define SINGLETIRESTATUS_INCLUDE


#include "SingleTirePressureStatus.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
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
