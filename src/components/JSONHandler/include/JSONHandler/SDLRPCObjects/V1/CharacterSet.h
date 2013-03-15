//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPC_CHARACTERSET_INCLUDE
#define NSSMARTDEVICELINKKRPC_CHARACTERSET_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPC
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
