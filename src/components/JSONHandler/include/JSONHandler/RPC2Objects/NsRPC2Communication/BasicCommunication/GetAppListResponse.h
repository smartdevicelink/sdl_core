#ifndef NSRPC2COMMUNICATION_APPLINKCORE_GETAPPLISTRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_GETAPPLISTRESPONSE_INCLUDE

#include <vector>
#include "JSONHandler/RPC2Response.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/HMIApplication.h"

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

    class GetAppListResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      GetAppListResponse(const GetAppListResponse& c);
      GetAppListResponse(void);
    
      GetAppListResponse& operator =(const GetAppListResponse&);
    
      virtual ~GetAppListResponse(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::vector< NsSmartDeviceLinkRPCV2::HMIApplication>& get_appList(void);


// setters
      bool set_appList(const std::vector< NsSmartDeviceLinkRPCV2::HMIApplication>& appList);


    private:

      friend class GetAppListResponseMarshaller;

      std::vector< NsSmartDeviceLinkRPCV2::HMIApplication> appList;

    };
  }
}

#endif
