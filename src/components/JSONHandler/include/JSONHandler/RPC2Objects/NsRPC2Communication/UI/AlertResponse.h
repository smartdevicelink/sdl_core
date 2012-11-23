#ifndef NSRPC2COMMUNICATION_UI_ALERTRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_UI_ALERTRESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


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

    class AlertResponse : public ::NsRPC2Communication::RPC2Response
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
