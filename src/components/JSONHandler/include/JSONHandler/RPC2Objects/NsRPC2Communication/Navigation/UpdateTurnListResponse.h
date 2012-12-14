#ifndef NSRPC2COMMUNICATION_NAVIGATION_UPDATETURNLISTRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_NAVIGATION_UPDATETURNLISTRESPONSE_INCLUDE

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

    class UpdateTurnListResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      UpdateTurnListResponse(const UpdateTurnListResponse& c);
      UpdateTurnListResponse(void);
    
      UpdateTurnListResponse& operator =(const UpdateTurnListResponse&);
    
      virtual ~UpdateTurnListResponse(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class UpdateTurnListResponseMarshaller;


    };
  }
}

#endif
