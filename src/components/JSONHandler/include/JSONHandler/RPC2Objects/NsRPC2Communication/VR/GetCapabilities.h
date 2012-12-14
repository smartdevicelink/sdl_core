#ifndef NSRPC2COMMUNICATION_VR_GETCAPABILITIES_INCLUDE
#define NSRPC2COMMUNICATION_VR_GETCAPABILITIES_INCLUDE

#include "JSONHandler/RPC2Request.h"


/*
  interface	NsRPC2Communication::VR
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace VR
  {

    class GetCapabilities : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      GetCapabilities(const GetCapabilities& c);
      GetCapabilities(void);
    
      GetCapabilities& operator =(const GetCapabilities&);
    
      virtual ~GetCapabilities(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class GetCapabilitiesMarshaller;


    };
  }
}

#endif
