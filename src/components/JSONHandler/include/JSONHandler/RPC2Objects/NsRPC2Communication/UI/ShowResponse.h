#ifndef NSRPC2COMMUNICATION_UI_SHOWRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_UI_SHOWRESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Nov 29 14:32:09 2012
  source stamp	Thu Nov 29 14:32:05 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class ShowResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      ShowResponse(const ShowResponse& c);
      ShowResponse(void);
    
      ShowResponse& operator =(const ShowResponse&);
    
      virtual ~ShowResponse(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class ShowResponseMarshaller;


    };
  }
}

#endif
