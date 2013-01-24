#ifndef NSAPPLINKRPCV2_AUDIOTYPE_INCLUDE
#define NSAPPLINKRPCV2_AUDIOTYPE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
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
