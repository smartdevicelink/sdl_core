#ifndef NSAPPLINKRPCV2_READDID_REQUEST_INCLUDE
#define NSAPPLINKRPCV2_READDID_REQUEST_INCLUDE

#include <vector>

#include "JSONHandler/ALRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

///  Non periodic vehicle data read request.

  class ReadDID_request : public NsAppLinkRPC::ALRPCMessage
  {
  public:
  
    ReadDID_request(const ReadDID_request& c);
    ReadDID_request(void);
    
    virtual ~ReadDID_request(void);
  
    ReadDID_request& operator =(const ReadDID_request&);
  
    bool checkIntegrity(void);

    unsigned int get_ecuName(void) const;
    const std::vector<unsigned int>& get_didLocation(void) const;
    const bool* get_encrypted(void) const;

    bool set_ecuName(unsigned int ecuName_);
    bool set_didLocation(const std::vector<unsigned int>& didLocation_);
    void reset_encrypted(void);
    bool set_encrypted(bool encrypted_);

  private:
  
    friend class ReadDID_requestMarshaller;


///  Name of ECU.
      unsigned int ecuName;	//!<  (0,65535)

///  Get raw data from vehicle data DID location(s).
      std::vector<unsigned int> didLocation;	//!<   [%s..%s]  (0,65535)

/**
     If not provided, the default is equal to False"
     Indicates if the DID data requested should be returned as encrypted through an OnEncodedSYNCPData response.
     If set to true, the data will return instead through OnEncodedSYNCPData and be passed through the proxy to the designated server URL.
*/
      bool* encrypted;
  };

}

#endif
