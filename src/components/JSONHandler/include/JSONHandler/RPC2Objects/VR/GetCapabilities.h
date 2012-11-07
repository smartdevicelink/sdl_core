#ifndef RPC2COMMUNICATION_VR_GETCAPABILITIES_INCLUDE
#define RPC2COMMUNICATION_VR_GETCAPABILITIES_INCLUDE

#include "JSONHandler/RPC2Request.h"


/*
  interface	RPC2Communication::VR
  version	1.2
  generated at	Wed Nov  7 11:26:04 2012
  source stamp	Wed Nov  7 09:31:14 2012
  author	robok0der
*/

namespace RPC2Communication
{
  namespace VR
  {

    class GetCapabilities : public ::RPC2Communication::RPC2Request
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
