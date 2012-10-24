#ifndef ONAUDIOPASSTHRU_INCLUDE
#define ONAUDIOPASSTHRU_INCLUDE


#include "DataType.h"
#include "../../../JSONHandler/ALRPCNotification.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


class OnAudioPassThru : public ALRPCNotification
{
public:

  OnAudioPassThru(const OnAudioPassThru& c);
  OnAudioPassThru(void);
  
  virtual ~OnAudioPassThru(void);

  bool checkIntegrity(void);

  const DataType& get_audioPacket(void) const;

  bool set_audioPacket(const DataType& audioPacket_);

private:

  friend class OnAudioPassThruMarshaller;

  DataType audioPacket;
};

#endif
