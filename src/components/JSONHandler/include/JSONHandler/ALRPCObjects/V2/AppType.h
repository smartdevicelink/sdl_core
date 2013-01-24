#ifndef NSAPPLINKRPCV2_APPTYPE_INCLUDE
#define NSAPPLINKRPCV2_APPTYPE_INCLUDE


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

///  Enumeration listing possible app types.

  class AppType
  {
  public:
    enum AppTypeInternal
    {
      INVALID_ENUM=-1,
      DEFAULT=0,
      COMMUNICATION=1,
      MEDIA=2,
      MESSAGING=3,
      NAVIGATION=4,
      INFORMATION=5,
      SOCIAL=6,
      BACKGROUND_PROCESS=7,
      TESTING=8,
      SYSTEM=9
    };
  
    AppType() : mInternal(INVALID_ENUM)				{}
    AppType(AppTypeInternal e) : mInternal(e)		{}
  
    AppTypeInternal get(void) const	{ return mInternal; }
    void set(AppTypeInternal e)		{ mInternal=e; }
  
  private:
    AppTypeInternal mInternal;
    friend class AppTypeMarshaller;
  };
  
}

#endif
