#ifndef NSAPPLINKRPC_APPTYPE_INCLUDE
#define NSAPPLINKRPC_APPTYPE_INCLUDE


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
