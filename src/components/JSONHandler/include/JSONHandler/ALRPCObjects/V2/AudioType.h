#ifndef NSAPPLINKRPCV2_AUDIOTYPE_INCLUDE
#define NSAPPLINKRPCV2_AUDIOTYPE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

///  Describes different audio type options for PerformAudioPassThru.

  class AudioType
  {
  public:
    enum AudioTypeInternal
    {
      INVALID_ENUM=-1,
      PCM=0
    };
  
    AudioType() : mInternal(INVALID_ENUM)				{}
    AudioType(AudioTypeInternal e) : mInternal(e)		{}
  
    AudioTypeInternal get(void) const	{ return mInternal; }
    void set(AudioTypeInternal e)		{ mInternal=e; }
  
  private:
    AudioTypeInternal mInternal;
    friend class AudioTypeMarshaller;
  };
  
}

#endif
