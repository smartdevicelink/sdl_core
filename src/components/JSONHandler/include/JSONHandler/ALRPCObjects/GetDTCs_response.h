#ifndef NSAPPLINKRPC_GETDTCS_RESPONSE_INCLUDE
#define NSAPPLINKRPC_GETDTCS_RESPONSE_INCLUDE

#include <vector>
#include <string>

#include "DTC.h"
#include "Result_v2.h"
#include "JSONHandler/ALRPCResponse.h"


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

  class GetDTCs_response : public ALRPCResponse
  {
  public:
  
    GetDTCs_response(const GetDTCs_response& c);
    GetDTCs_response(void);
    
    virtual ~GetDTCs_response(void);
  
    GetDTCs_response& operator =(const GetDTCs_response&);
  
    bool checkIntegrity(void);

    bool get_success(void) const;
    const std::vector<Result_v2>& get_resultCode(void) const;
    const std::string* get_info(void) const;
    const std::vector<DTC>* get_dtcList(void) const;

    bool set_success(bool success_);
    bool set_resultCode(const std::vector<Result_v2>& resultCode_);
    void reset_info(void);
    bool set_info(const std::string& info_);
    void reset_dtcList(void);
    bool set_dtcList(const std::vector<DTC>& dtcList_);

  private:
  
    friend class GetDTCs_responseMarshaller;


/**
     true, if successful
     false, if failed
*/
      bool success;

///  See Result
      std::vector<Result_v2> resultCode;	//!<   [%s..%s] 

///  Provides additional human readable info regarding the result.
      std::string* info;	//!< (1000)

///  Array of all reported DTCs on module.
      std::vector<DTC>* dtcList;	//!<   [%s..%s] 
  };

}

#endif
