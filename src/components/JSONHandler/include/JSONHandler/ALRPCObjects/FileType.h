#ifndef NSAPPLINKRPC_FILETYPE_INCLUDE
#define NSAPPLINKRPC_FILETYPE_INCLUDE


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

///  Enumeration listing possible file types.

  class FileType
  {
  public:
    enum FileTypeInternal
    {
      INVALID_ENUM=-1,
      GRAPHIC_BMP=0,
      GRAPHIC_JPEG=1,
      GRAPHIC_PNG=2,
      AUDIO_WAVE=3,
      AUDIO_MP3=4
    };
  
    FileType() : mInternal(INVALID_ENUM)				{}
    FileType(FileTypeInternal e) : mInternal(e)		{}
  
    FileTypeInternal get(void) const	{ return mInternal; }
    void set(FileTypeInternal e)		{ mInternal=e; }
  
  private:
    FileTypeInternal mInternal;
    friend class FileTypeMarshaller;
  };
  
}

#endif
