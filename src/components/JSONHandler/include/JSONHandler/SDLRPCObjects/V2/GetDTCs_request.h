#ifndef NSAPPLINKRPCV2_GETDTCS_REQUEST_INCLUDE
#define NSAPPLINKRPCV2_GETDTCS_REQUEST_INCLUDE


#include "JSONHandler/SDLRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsSmartDeviceLinkRPCV2
{

///  Vehicle module diagnostic trouble code request.

  class GetDTCs_request : public NsSmartDeviceLinkRPC::SDLRPCMessage
  {
  public:
  
    GetDTCs_request(const GetDTCs_request& c);
    GetDTCs_request(void);
    
    virtual ~GetDTCs_request(void);
  
    GetDTCs_request& operator =(const GetDTCs_request&);
  
    bool checkIntegrity(void);

    unsigned int get_ecuName(void) const;
    const bool* get_encrypted(void) const;

    bool set_ecuName(unsigned int ecuName_);
    void reset_encrypted(void);
    bool set_encrypted(bool encrypted_);

  private:
  
    friend class GetDTCs_requestMarshaller;


///  Name of ECU.
      unsigned int ecuName;	//!<  (0,65535)

/**
     If not provided, the default is equal to False"
     Indicates if the DTC data requested should be returned as encrypted through an OnEncodedSYNCPData response.
     If set to true, the data will return instead through OnEncodedSYNCPData and be passed through the proxy to the designated server URL.
*/
      bool* encrypted;
  };

}

#endif
