#ifndef CHARACTERSET_INCLUDE
#define CHARACTERSET_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


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

#endif
