#ifndef NSAPPLINKRPC_SETAPPICON_REQUEST_INCLUDE
#define NSAPPLINKRPC_SETAPPICON_REQUEST_INCLUDE

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
     Used to set existing local file on SYNC as the app's icon
     Not supported on first generation SYNC vehicles. 
*/

  class SetAppIcon_request : public ALRPCRequest
  {
  public:
  
    SetAppIcon_request(const SetAppIcon_request& c);
    SetAppIcon_request(void);
    
    virtual ~SetAppIcon_request(void);
  
    SetAppIcon_request& operator =(const SetAppIcon_request&);
  
    bool checkIntegrity(void);

    const std::string* get_syncFileName(void) const;

    void reset_syncFileName(void);
    bool set_syncFileName(const std::string& syncFileName_);

  private:
  
    friend class SetAppIcon_requestMarshaller;


///  File reference name.
      std::string* syncFileName;	//!< (500)
  };

}

#endif
