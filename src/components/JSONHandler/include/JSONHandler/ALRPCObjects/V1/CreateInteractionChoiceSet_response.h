#ifndef NSAPPLINKRPC_CREATEINTERACTIONCHOICESET_RESPONSE_INCLUDE
#define NSAPPLINKRPC_CREATEINTERACTIONCHOICESET_RESPONSE_INCLUDE

#include <string>

#include "Result.h"
#include "JSONHandler/ALRPCResponse.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	robok0der
*/

namespace NsAppLinkRPC
{

  class CreateInteractionChoiceSet_response : public ALRPCResponse
  {
  public:
  
    CreateInteractionChoiceSet_response(const CreateInteractionChoiceSet_response& c);
    CreateInteractionChoiceSet_response(void);
    
    virtual ~CreateInteractionChoiceSet_response(void);
  
    CreateInteractionChoiceSet_response& operator =(const CreateInteractionChoiceSet_response&);
  
    bool checkIntegrity(void);

    bool get_success(void) const;
    const Result& get_resultCode(void) const;
    const std::string* get_info(void) const;

    bool set_success(bool success_);
    bool set_resultCode(const Result& resultCode_);
    void reset_info(void);
    bool set_info(const std::string& info_);

  private:
  
    friend class CreateInteractionChoiceSet_responseMarshaller;


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
