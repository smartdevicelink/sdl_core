#ifndef NSRPC2COMMUNICATION_UI_ONSYSTEMCONTEXT_INCLUDE
#define NSRPC2COMMUNICATION_UI_ONSYSTEMCONTEXT_INCLUDE

#include "JSONHandler/RPC2Notification.h"

#include "../include/JSONHandler/ALRPCObjects/SystemContext.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Nov 20 13:32:23 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class OnSystemContext : public ::NsRPC2Communication::RPC2Notification
    {
    public:
    
      OnSystemContext(const OnSystemContext& c);
      OnSystemContext(void);
    
      OnSystemContext& operator =(const OnSystemContext&);
    
      virtual ~OnSystemContext(void);
    
      bool checkIntegrity(void);
    
// getters
      const NsAppLinkRPC::SystemContext& get_systemContext(void);


// setters
      bool set_systemContext(const NsAppLinkRPC::SystemContext& systemContext);


    private:

      friend class OnSystemContextMarshaller;

      NsAppLinkRPC::SystemContext systemContext;

    };
  }
}

#endif
