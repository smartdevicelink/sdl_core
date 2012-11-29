#ifndef NSAPPLINKRPC_UPDATETURNLIST_RESPONSE_INCLUDE
#define NSAPPLINKRPC_UPDATETURNLIST_RESPONSE_INCLUDE

#include <string>

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

  class UpdateTurnList_response : public ALRPCResponse
  {
  public:
  
    UpdateTurnList_response(const UpdateTurnList_response& c);
    UpdateTurnList_response(void);
    
    virtual ~UpdateTurnList_response(void);
  
    UpdateTurnList_response& operator =(const UpdateTurnList_response&);
  
    bool checkIntegrity(void);

    bool get_success(void) const;
    const Result_v2& get_resultCode(void) const;
    const std::string* get_info(void) const;

    bool set_success(bool success_);
    bool set_resultCode(const Result_v2& resultCode_);
    void reset_info(void);
    bool set_info(const std::string& info_);

  private:
  
    friend class UpdateTurnList_responseMarshaller;


/**
     true, if successful
     false, if failed
*/
      bool success;

///  See Result
      Result_v2 resultCode;

///  Provides additional human readable info regarding the result.
      std::string* info;	//!< (1000)
  };

}

#endif
