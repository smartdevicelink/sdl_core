#ifndef DATATYPE_INCLUDE
#define DATATYPE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/


///  Contains information about the type of data.

class DataType
{
public:
  enum DataTypeInternal
  {
    INVALID_ENUM=-1,
    HEX=0,
    BINARY=1
  };

  DataType() : mInternal(INVALID_ENUM)				{}
  DataType(DataTypeInternal e) : mInternal(e)		{}

  DataTypeInternal get(void) const	{ return mInternal; }
  void set(DataTypeInternal e)		{ mInternal=e; }

private:
  DataTypeInternal mInternal;
  friend class DataTypeMarshaller;
};

#endif
