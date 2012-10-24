#ifndef FILETYPE_INCLUDE
#define FILETYPE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


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

#endif
