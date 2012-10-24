#ifndef DELETEFILE_REQUEST_INCLUDE
#define DELETEFILE_REQUEST_INCLUDE

#include <string>

#include "../../../JSONHandler/ALRPCRequest.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


/**
     Used to delete a file resident on the SYNC module in the app's local cache.
     Not supported on first generation SYNC vehicles. 
*/

class DeleteFile_request : public ALRPCRequest
{
public:

  DeleteFile_request(const DeleteFile_request& c);
  DeleteFile_request(void);
  
  virtual ~DeleteFile_request(void);

  DeleteFile_request& operator =(const DeleteFile_request&);

  bool checkIntegrity(void);

  const std::string* get_syncFileName(void) const;

  void reset_syncFileName(void);
  bool set_syncFileName(const std::string& syncFileName_);

private:

  friend class DeleteFile_requestMarshaller;

  std::string* syncFileName;	//!< (500)
};

#endif
