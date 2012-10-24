#ifndef SYSTEMCONTEXT_INCLUDE
#define SYSTEMCONTEXT_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


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

#endif
