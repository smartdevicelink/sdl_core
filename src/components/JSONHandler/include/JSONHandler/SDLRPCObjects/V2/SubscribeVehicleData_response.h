//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_SUBSCRIBEVEHICLEDATA_RESPONSE_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_SUBSCRIBEVEHICLEDATA_RESPONSE_INCLUDE

#include <vector>

#include "VehicleDataResult.h"
#include "JSONHandler/SDLRPCResponse.h"

/*
  interface Ford Sync RAPI
  version   2.0O
  date      2012-11-02
  generated at  Thu Jan 24 06:36:23 2013
  source stamp  Thu Jan 24 06:35:41 2013
  author    RC
*/

namespace NsSmartDeviceLinkRPCV2
{
    class SubscribeVehicleData_response : public NsSmartDeviceLinkRPC::SDLRPCResponse
    {
    public:
        SubscribeVehicleData_response(const SubscribeVehicleData_response& c);
        SubscribeVehicleData_response(void);

        virtual ~SubscribeVehicleData_response(void);

        SubscribeVehicleData_response& operator =(const SubscribeVehicleData_response&);

        bool checkIntegrity(void);

        const std::vector<VehicleDataResult>* get_dataResult(void) const;
        void reset_dataResult(void);
        bool set_dataResult(const std::vector<VehicleDataResult>& dataResult_);

    private:
        friend class SubscribeVehicleData_responseMarshaller;

        ///  Permission status of vehicle data element.
        std::vector<VehicleDataResult>* dataResult;   //!<   [%s..%s]
    };
}

#endif
