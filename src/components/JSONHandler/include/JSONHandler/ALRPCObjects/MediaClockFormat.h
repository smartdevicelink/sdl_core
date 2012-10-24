#ifndef MEDIACLOCKFORMAT_INCLUDE
#define MEDIACLOCKFORMAT_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


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

#endif
