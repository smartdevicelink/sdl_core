#ifndef NSAPPLINKRPC_SYSTEMCONTEXT_INCLUDE
#define NSAPPLINKRPC_SYSTEMCONTEXT_INCLUDE


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

///  Enumeraction that describes possible contexts and app's HMI might be in.

  class SystemContext
  {
  public:
    enum SystemContextInternal
    {
      INVALID_ENUM=-1,
      SYSCTXT_MAIN=0,
      SYSCTXT_VRSESSION=1,
      SYSCTXT_MENU=2
    };
  
    SystemContext() : mInternal(INVALID_ENUM)				{}
    SystemContext(SystemContextInternal e) : mInternal(e)		{}
  
    SystemContextInternal get(void) const	{ return mInternal; }
    void set(SystemContextInternal e)		{ mInternal=e; }
  
  private:
    SystemContextInternal mInternal;
    friend class SystemContextMarshaller;
  };
  
}

#endif
