#ifndef NSRPC2COMMUNICATION_NAVIGATION_SHOWCONSTANTTBTRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_NAVIGATION_SHOWCONSTANTTBTRESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	NsRPC2Communication::Navigation
  version	2.0
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace Navigation
  {

    class ShowConstantTBTResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      ShowConstantTBTResponse(const ShowConstantTBTResponse& c);
      ShowConstantTBTResponse(void);
    
      ShowConstantTBTResponse& operator =(const ShowConstantTBTResponse&);
    
      virtual ~ShowConstantTBTResponse(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class ShowConstantTBTResponseMarshaller;


    };
  }
}

#endif
