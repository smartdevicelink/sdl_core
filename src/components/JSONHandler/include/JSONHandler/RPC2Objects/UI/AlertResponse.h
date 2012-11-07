#ifndef RPC2COMMUNICATION_UI_ALERTRESPONSE_INCLUDE
#define RPC2COMMUNICATION_UI_ALERTRESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


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

    class AlertResponse : public ::RPC2Communication::RPC2Response
    {
    public:
    
      AlertResponse(const AlertResponse& c);
      AlertResponse(void);
    
      AlertResponse& operator =(const AlertResponse&);
    
      virtual ~AlertResponse(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class AlertResponseMarshaller;


    };
  }
}

#endif
