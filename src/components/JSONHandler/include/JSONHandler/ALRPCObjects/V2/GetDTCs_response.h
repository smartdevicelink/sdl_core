#ifndef NSAPPLINKRPCV2_GETDTCS_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_GETDTCS_RESPONSE_INCLUDE

#include <vector>
#include <string>

#include "DTC.h"
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

  class GetDTCs_response : public ALRPC2Message
  {
  public:
  
    GetDTCs_response(const GetDTCs_response& c);
    GetDTCs_response(void);
    
    virtual ~GetDTCs_response(void);
  
    GetDTCs_response& operator =(const GetDTCs_response&);
  
    bool checkIntegrity(void);

    bool get_success(void) const;
    const std::vector<Result>& get_resultCode(void) const;
    const std::string* get_info(void) const;
    const std::vector<DTC>* get_dtcList(void) const;

    bool set_success(bool success_);
    bool set_resultCode(const std::vector<Result>& resultCode_);
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
      std::vector<Result> resultCode;	//!<   [%s..%s] 

///  Provides additional human readable info regarding the result.
      std::string* info;	//!< (1000)

///  Array of all reported DTCs on module.
      std::vector<DTC>* dtcList;	//!<   [%s..%s] 
  };

}

#endif
