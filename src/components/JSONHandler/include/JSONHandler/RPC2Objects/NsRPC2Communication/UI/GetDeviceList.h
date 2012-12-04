#ifndef NSRPC2COMMUNICATION_UI_GETDEVICELIST_INCLUDE
#define NSRPC2COMMUNICATION_UI_GETDEVICELIST_INCLUDE

#include "JSONHandler/RPC2Request.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Dec  4 15:06:30 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
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
