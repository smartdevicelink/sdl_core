#ifndef NSRPC2COMMUNICATION_APPLINKCORE_GETDEVICELISTRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_GETDEVICELISTRESPONSE_INCLUDE

#include <string>
#include <vector>
#include "JSONHandler/RPC2Response.h"


/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace AppLinkCore
  {

    class GetDeviceListResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      GetDeviceListResponse(const GetDeviceListResponse& c);
      GetDeviceListResponse(void);
    
      GetDeviceListResponse& operator =(const GetDeviceListResponse&);
    
      virtual ~GetDeviceListResponse(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::vector< std::string>& get_deviceList(void);


// setters
/// 1 <= size <= 100
      bool set_deviceList(const std::vector< std::string>& deviceList);


    private:

      friend class GetDeviceListResponseMarshaller;

      std::vector< std::string> deviceList;

    };
  }
}

#endif
