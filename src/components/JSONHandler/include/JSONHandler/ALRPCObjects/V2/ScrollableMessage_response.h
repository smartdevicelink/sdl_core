#ifndef NSAPPLINKRPCV2_SCROLLABLEMESSAGE_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_SCROLLABLEMESSAGE_RESPONSE_INCLUDE


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

  class ScrollableMessage_response : public ALRPC2Message
  {
  public:
  
    ScrollableMessage_response(const ScrollableMessage_response& c);
    ScrollableMessage_response(void);
    
    virtual ~ScrollableMessage_response(void);
  
    bool checkIntegrity(void);

    bool get_success(void) const;
    const Result& get_resultCode(void) const;

    bool set_success(bool success_);
    bool set_resultCode(const Result& resultCode_);

  private:
  
    friend class ScrollableMessage_responseMarshaller;


/**
     true, if successful
     false, if failed
*/
      bool success;

///  See Result
      Result resultCode;
  };

}

#endif
