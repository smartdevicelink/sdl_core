#ifndef NSAPPLINKRPCV2_READDID_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_READDID_RESPONSE_INCLUDE

#include <vector>
#include <string>

#include "Result.h"
#include "VehicleDataResultCode.h"
#include "JSONHandler/ALRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  class ReadDID_response : public NsAppLinkRPC::ALRPCMessage
  {
  public:
  
    ReadDID_response(const ReadDID_response& c);
    ReadDID_response(void);
    
    virtual ~ReadDID_response(void);
  
    ReadDID_response& operator =(const ReadDID_response&);
  
    bool checkIntegrity(void);

    bool get_success(void) const;
    const std::vector<Result>& get_resultCode(void) const;
    const std::string* get_info(void) const;
    const std::vector<VehicleDataResultCode>* get_dataResult(void) const;
    const std::vector<std::string>* get_data(void) const;

    bool set_success(bool success_);
    bool set_resultCode(const std::vector<Result>& resultCode_);
    void reset_info(void);
    bool set_info(const std::string& info_);
    void reset_dataResult(void);
    bool set_dataResult(const std::vector<VehicleDataResultCode>& dataResult_);
    void reset_data(void);
    bool set_data(const std::vector<std::string>& data_);

  private:
  
    friend class ReadDID_responseMarshaller;


/**
     true, if successful
     false, if failed
*/
      bool success;

///  See Result
      std::vector<Result> resultCode;	//!<   [%s..%s] 

///  Provides additional human readable info regarding the result.
      std::string* info;	//!< (1000)

///  Permission status of data element.
      std::vector<VehicleDataResultCode>* dataResult;	//!<   [%s..%s] 

///  Raw DID-based data returned for requested element.
      std::vector<std::string>* data;	//!<   [%s..%s] (5000)
  };

}

#endif
