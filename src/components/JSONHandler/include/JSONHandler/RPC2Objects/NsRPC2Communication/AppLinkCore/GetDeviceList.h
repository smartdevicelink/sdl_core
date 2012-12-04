#ifndef NSRPC2COMMUNICATION_APPLINKCORE_GETDEVICELIST_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_GETDEVICELIST_INCLUDE

#include "JSONHandler/RPC2Request.h"


/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace AppLinkCore
  {

    class GetDeviceList : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      GetDeviceList(const GetDeviceList& c);
      GetDeviceList(void);
    
      GetDeviceList& operator =(const GetDeviceList&);
    
      virtual ~GetDeviceList(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class GetDeviceListMarshaller;


    };
  }
}

#endif
