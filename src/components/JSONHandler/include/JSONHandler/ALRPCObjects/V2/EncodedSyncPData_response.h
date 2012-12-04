#ifndef NSAPPLINKRPCV2_ENCODEDSYNCPDATA_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_ENCODEDSYNCPDATA_RESPONSE_INCLUDE

#include <string>

#include "Result.h"
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

  class EncodedSyncPData_response : public ALRPC2Message
  {
  public:
  
    EncodedSyncPData_response(const EncodedSyncPData_response& c);
    EncodedSyncPData_response(void);
    
    virtual ~EncodedSyncPData_response(void);
  
    EncodedSyncPData_response& operator =(const EncodedSyncPData_response&);
  
    bool checkIntegrity(void);

    bool get_success(void) const;
    const Result& get_resultCode(void) const;
    const std::string* get_info(void) const;

    bool set_success(bool success_);
    bool set_resultCode(const Result& resultCode_);
    void reset_info(void);
    bool set_info(const std::string& info_);

  private:
  
    friend class EncodedSyncPData_responseMarshaller;


/**
     true, if successful
     false, if failed
*/
      bool success;

///  See Result
      Result resultCode;

///  Provides additional human readable info regarding the result.
      std::string* info;	//!< (1000)
  };

}

#endif
