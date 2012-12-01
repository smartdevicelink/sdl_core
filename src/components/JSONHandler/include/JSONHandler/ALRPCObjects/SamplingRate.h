#ifndef NSAPPLINKRPC_SAMPLINGRATE_INCLUDE
#define NSAPPLINKRPC_SAMPLINGRATE_INCLUDE


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

///  Describes different sampling options for PerformAudioPassThru.

  class SamplingRate
  {
  public:
    enum SamplingRateInternal
    {
      INVALID_ENUM=-1,
      FIX_8KHZ=0,
      FIX_16KHZ=1,
      FIX_22KHZ=2,
      FIX_44KHZ=3
    };
  
    SamplingRate() : mInternal(INVALID_ENUM)				{}
    SamplingRate(SamplingRateInternal e) : mInternal(e)		{}
  
    SamplingRateInternal get(void) const	{ return mInternal; }
    void set(SamplingRateInternal e)		{ mInternal=e; }
  
  private:
    SamplingRateInternal mInternal;
    friend class SamplingRateMarshaller;
  };
  
}

#endif
