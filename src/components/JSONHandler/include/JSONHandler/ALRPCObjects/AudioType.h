#ifndef NSAPPLINKRPC_AUDIOTYPE_INCLUDE
#define NSAPPLINKRPC_AUDIOTYPE_INCLUDE


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
