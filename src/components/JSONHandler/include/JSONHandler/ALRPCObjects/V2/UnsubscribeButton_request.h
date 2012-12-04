#ifndef NSAPPLINKRPCV2_UNSUBSCRIBEBUTTON_REQUEST_INCLUDE
#define NSAPPLINKRPCV2_UNSUBSCRIBEBUTTON_REQUEST_INCLUDE


#include "ButtonName.h"
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

///  Unsubscribes from built-in HMI buttons.

  class UnsubscribeButton_request : public ALRPC2Message
  {
  public:
  
    UnsubscribeButton_request(const UnsubscribeButton_request& c);
    UnsubscribeButton_request(void);
    
    virtual ~UnsubscribeButton_request(void);
  
    bool checkIntegrity(void);

    const ButtonName& get_buttonName(void) const;

    bool set_buttonName(const ButtonName& buttonName_);

  private:
  
    friend class UnsubscribeButton_requestMarshaller;


///  Name of the button to unsubscribe.
      ButtonName buttonName;
  };

}

#endif
