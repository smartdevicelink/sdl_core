#ifndef NSAPPLINKRPC_PERFORMINTERACTION_RESPONSE_INCLUDE
#define NSAPPLINKRPC_PERFORMINTERACTION_RESPONSE_INCLUDE

#include <string>

#include "Result.h"
#include "TriggerSource.h"
#include "JSONHandler/ALRPCResponse.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Dec 13 13:37:09 2012
  source stamp	Thu Dec 13 13:33:23 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

  class PerformInteraction_response : public ALRPCResponse
  {
  public:
  
    PerformInteraction_response(const PerformInteraction_response& c);
    PerformInteraction_response(void);
    
    virtual ~PerformInteraction_response(void);
  
    PerformInteraction_response& operator =(const PerformInteraction_response&);
  
    bool checkIntegrity(void);

    bool get_success(void) const;
    const Result& get_resultCode(void) const;
    const std::string* get_info(void) const;
    const unsigned int* get_choiceID(void) const;
    const TriggerSource* get_triggerSource(void) const;

    bool set_success(bool success_);
    bool set_resultCode(const Result& resultCode_);
    void reset_info(void);
    bool set_info(const std::string& info_);
    void reset_choiceID(void);
    bool set_choiceID(unsigned int choiceID_);
    void reset_triggerSource(void);
    bool set_triggerSource(const TriggerSource& triggerSource_);

  private:
  
    friend class PerformInteraction_responseMarshaller;


/**
     true, if successful
     false, if failed
*/
      bool success;

///  See Result
      Result resultCode;

///  Provides additional human readable info regarding the result.
      std::string* info;	//!< (1000)

///  ID of the choice that was selected in response to PerformInteraction.
      unsigned int* choiceID;	//!<  (0,2000000000)

///  See TriggerSource
      TriggerSource* triggerSource;
  };

}

#endif
