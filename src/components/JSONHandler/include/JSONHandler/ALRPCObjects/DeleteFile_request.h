#ifndef NSAPPLINKRPC_DELETEFILE_REQUEST_INCLUDE
#define NSAPPLINKRPC_DELETEFILE_REQUEST_INCLUDE

#include <string>

#include "JSONHandler/ALRPCRequest.h"


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


///  File reference name.
      std::string* syncFileName;	//!< (500)
  };

}

#endif
