#ifndef NSAPPLINKRPCV2_CHARACTERSET_INCLUDE
#define NSAPPLINKRPCV2_CHARACTERSET_INCLUDE


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
