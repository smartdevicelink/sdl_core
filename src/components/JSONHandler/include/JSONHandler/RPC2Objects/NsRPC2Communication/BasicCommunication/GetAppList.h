#ifndef NSRPC2COMMUNICATION_APPLINKCORE_GETAPPLIST_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_GETAPPLIST_INCLUDE

#include "JSONHandler/RPC2Request.h"


/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace BasicCommunication
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
