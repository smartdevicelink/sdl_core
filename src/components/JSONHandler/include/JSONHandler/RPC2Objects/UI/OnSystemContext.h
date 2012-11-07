#ifndef RPC2COMMUNICATION_UI_ONSYSTEMCONTEXT_INCLUDE
#define RPC2COMMUNICATION_UI_ONSYSTEMCONTEXT_INCLUDE

#include "JSONHandler/RPC2Notification.h"

#include "../include/JSONHandler/ALRPCObjects/SystemContext.h"

/*
  interface	RPC2Communication::UI
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

namespace RPC2Communication
{
  namespace UI
  {

    class OnSystemContext : public ::RPC2Communication::RPC2Notification
    {
    public:
    
      OnSystemContext(const OnSystemContext& c);
      OnSystemContext(void);
    
      OnSystemContext& operator =(const OnSystemContext&);
    
      virtual ~OnSystemContext(void);
    
      bool checkIntegrity(void);
    
// getters
      const ::AppLinkRPC::SystemContext& get_systemContext(void);


// setters
      bool set_systemContext(const ::AppLinkRPC::SystemContext& systemContext);


    private:

      friend class OnSystemContextMarshaller;

      ::AppLinkRPC::SystemContext systemContext;

    };
  }
}

#endif
