//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_MEDIACLOCKFORMAT_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_MEDIACLOCKFORMAT_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPCV2
{

  class MediaClockFormat
  {
  public:
    enum MediaClockFormatInternal
    {
      INVALID_ENUM=-1,

/**
     minutesFieldWidth = 2;minutesFieldMax = 19;secondsFieldWidth = 2;secondsFieldMax = 99;maxHours = 19;maxMinutes = 59;maxSeconds = 59;
     used for Type II and CID headunits
*/
      CLOCK1=0,

/**
     minutesFieldWidth = 3;minutesFieldMax = 199;secondsFieldWidth = 2;secondsFieldMax = 99;maxHours = 59;maxMinutes = 59;maxSeconds = 59;
     used for Type V headunit
*/
      CLOCK2=1,

/**
     5 characters possible
     Format:      1|sp   c   :|sp   c   c
     1|sp : digit "1" or space
     c    : character out of following character set: sp|0-9|[letters, see TypeII column in XLS. See [@TODO: create file ref]]
     :|sp : colon or space
     used for Type II headunit
*/
      CLOCKTEXT1=2,

/**
     5 chars possible
     Format:      1|sp   c   :|sp   c   c
     1|sp : digit "1" or space
     c    : character out of following character set: sp|0-9|[letters, see CID column in XLS. See [@TODO: create file ref]]
     :|sp : colon or space
     used for CID headunit
     NOTE: difference between CLOCKTEXT1 and CLOCKTEXT2 is the supported character set
*/
      CLOCKTEXT2=3,

/**
     6 chars possible
     Format:      1|sp   c   c   :|sp   c   c
     1|sp : digit "1" or space
     c    : character out of following character set: sp|0-9|[letters, see Type 5 column in XLS]. See [@TODO: create file ref]
     :|sp : colon or space
     used for Type V headunit
*/
      CLOCKTEXT3=4
    };
  
    MediaClockFormat() : mInternal(INVALID_ENUM)				{}
    MediaClockFormat(MediaClockFormatInternal e) : mInternal(e)		{}
  
    MediaClockFormatInternal get(void) const	{ return mInternal; }
    void set(MediaClockFormatInternal e)		{ mInternal=e; }
  
  private:
    MediaClockFormatInternal mInternal;
    friend class MediaClockFormatMarshaller;
  };
  
}

#endif
