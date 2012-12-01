#ifndef NSAPPLINKRPC_CHARACTERSET_INCLUDE
#define NSAPPLINKRPC_CHARACTERSET_INCLUDE


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

///  The list of potential character sets

  class CharacterSet
  {
  public:
    enum CharacterSetInternal
    {
      INVALID_ENUM=-1,

///  See [@TODO: create file ref]
      TYPE2SET=0,

///  See [@TODO: create file ref]
      TYPE5SET=1,

///  See [@TODO: create file ref]
      CID1SET=2,

///  See [@TODO: create file ref]
      CID2SET=3
    };
  
    CharacterSet() : mInternal(INVALID_ENUM)				{}
    CharacterSet(CharacterSetInternal e) : mInternal(e)		{}
  
    CharacterSetInternal get(void) const	{ return mInternal; }
    void set(CharacterSetInternal e)		{ mInternal=e; }
  
  private:
    CharacterSetInternal mInternal;
    friend class CharacterSetMarshaller;
  };
  
}

#endif
