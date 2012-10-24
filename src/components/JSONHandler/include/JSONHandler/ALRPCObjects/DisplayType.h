#ifndef DISPLAYTYPE_INCLUDE
#define DISPLAYTYPE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


///  See DAES for further infos regarding the displays]

class DisplayType
{
public:
  enum DisplayTypeInternal
  {
    INVALID_ENUM=-1,
    CID=0,
    TYPE2=1,
    TYPE5=2,
    NGN=3,
    GEN2_8_DMA=4,
    GEN2_6_DMA=5,
    MFD3=6,
    MFD4=7,
    MFD5=8
  };

  DisplayType() : mInternal(INVALID_ENUM)				{}
  DisplayType(DisplayTypeInternal e) : mInternal(e)		{}

  DisplayTypeInternal get(void) const	{ return mInternal; }
  void set(DisplayTypeInternal e)		{ mInternal=e; }

private:
  DisplayTypeInternal mInternal;
  friend class DisplayTypeMarshaller;
};

#endif
