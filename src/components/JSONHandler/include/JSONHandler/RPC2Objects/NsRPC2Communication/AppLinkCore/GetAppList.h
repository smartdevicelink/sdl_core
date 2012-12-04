#ifndef NSRPC2COMMUNICATION_APPLINKCORE_GETAPPLIST_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_GETAPPLIST_INCLUDE

#include "JSONHandler/RPC2Request.h"


/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Tue Dec  4 15:06:30 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace AppLinkCore
  {

    class GetAppList : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      GetAppList(const GetAppList& c);
      GetAppList(void);
    
      GetAppList& operator =(const GetAppList&);
    
      virtual ~GetAppList(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class GetAppListMarshaller;


    };
  }
}

#endif
