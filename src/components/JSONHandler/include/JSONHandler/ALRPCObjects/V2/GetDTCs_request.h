#ifndef NSAPPLINKRPCV2_GETDTCS_REQUEST_INCLUDE
#define NSAPPLINKRPCV2_GETDTCS_REQUEST_INCLUDE


#include "JSONHandler/ALRPC2Message.hh"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

///  Vehicle module diagnostic trouble code request.

  class GetDTCs_request : public ALRPC2Message
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
