#ifndef BLOB_HPP_
#define BLOB_HPP_

#include "../../utils/misc/Types.hpp"

class Blob
{
public:
   Blob(UInt32 id)
      : mSize(0)
   	  , mpBuf(NULL)
   	  , mId(id)
   { }

   Blob(UInt8 * pBuffer, size_t size, UInt32 id)
   	   : mSize(size)
   	   , mpBuf(pBuffer)
   	   , mId(id)
   { }

   const UInt8 * buffer() const
   {
	   return mpBuf;
   }

   size_t size() const
   {
	   return mSize;
   }

   void setBuffer(UInt8 * pBuffer, size_t size)
   {
      mpBuf = pBuffer;
      mSize = size;
   }

   UInt32 id() const
   {
	   return mId;
   }

private:
   size_t mSize;
   UInt8 * mpBuf;
   UInt32 mId;
};


#endif /* BLOB_HPP_ */
