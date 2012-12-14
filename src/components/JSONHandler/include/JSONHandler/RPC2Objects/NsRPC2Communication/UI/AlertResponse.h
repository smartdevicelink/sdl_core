#ifndef NSRPC2COMMUNICATION_UI_ALERTRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_UI_ALERTRESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
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
    
// getters
      const unsigned int* get_tryAgainTime(void);

// setters
/// tryAgainTime <= 2000000000
      bool set_tryAgainTime(const unsigned int& tryAgainTime);

      void reset_tryAgainTime(void);


    private:

      friend class AlertResponseMarshaller;

      unsigned int* tryAgainTime;

    };
  }
}

#endif
