#ifndef NSAPPLINKRPCV2_UNSUBSCRIBEVEHICLEDATA_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_UNSUBSCRIBEVEHICLEDATA_RESPONSE_INCLUDE

#include <vector>

#include "VehicleDataResult.h"
#include "JSONHandler/SDLRPCResponse.h"

/*
  interface Ford Sync RAPI
  version   2.0O
  date      2012-11-02
  generated at  Thu Jan 24 06:36:23 2013
  source stamp  Thu Jan 24 06:35:41 2013
  author    robok0der
*/

namespace NsSmartDeviceLinkRPCV2
{
    class UnsubscribeVehicleData_response : public NsSmartDeviceLinkRPC::SDLRPCResponse
    {
    public:
        UnsubscribeVehicleData_response(const UnsubscribeVehicleData_response& c);
        UnsubscribeVehicleData_response(void);

        virtual ~UnsubscribeVehicleData_response(void);

        UnsubscribeVehicleData_response& operator =(const UnsubscribeVehicleData_response&);

        bool checkIntegrity(void);

        const std::vector<VehicleDataResult>* get_dataResult(void) const;
        void reset_dataResult(void);
        bool set_dataResult(const std::vector<VehicleDataResult>& dataResult_);

    private:
        friend class UnsubscribeVehicleData_responseMarshaller;

        ///  Permission status of vehicle data element.
        std::vector<VehicleDataResult>* dataResult;   //!<   [%s..%s]
    };
}

#endif
