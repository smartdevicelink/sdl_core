#ifndef NSRPC2COMMUNICATION_VEHICLEINFO_READDIDRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_VEHICLEINFO_READDIDRESPONSE_INCLUDE

#include <string>
#include <vector>
#include "JSONHandler/RPC2Response.h"

#include "../include/JSONHandler/ALRPCObjects/V2/VehicleDataResultCode.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace VehicleInfo
  {

    class ReadDIDResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      ReadDIDResponse(const ReadDIDResponse& c);
      ReadDIDResponse(void);
    
      ReadDIDResponse& operator =(const ReadDIDResponse&);
    
      virtual ~ReadDIDResponse(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::vector< NsAppLinkRPCV2::VehicleDataResultCode>* get_dataResult(void);
      const std::vector< std::string>* get_data(void);

// setters
/// 0 <= size <= 1000
      bool set_dataResult(const std::vector< NsAppLinkRPCV2::VehicleDataResultCode>& dataResult);

      void reset_dataResult(void);

/// data[] <= 5000 ; 0 <= size <= 1000
      bool set_data(const std::vector< std::string>& data);

      void reset_data(void);


    private:

      friend class ReadDIDResponseMarshaller;

      std::vector< NsAppLinkRPCV2::VehicleDataResultCode>* dataResult;
      std::vector< std::string>* data;

    };
  }
}

#endif
