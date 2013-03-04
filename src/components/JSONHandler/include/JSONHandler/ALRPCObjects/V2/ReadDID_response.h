#ifndef NSAPPLINKRPCV2_READDID_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_READDID_RESPONSE_INCLUDE

#include <vector>
#include <string>

#include "VehicleDataResultCode.h"
#include "JSONHandler/ALRPCResponse.h"

/*
  interface Ford Sync RAPI
  version   2.0O
  date      2012-11-02
  generated at  Thu Jan 24 06:36:23 2013
  source stamp  Thu Jan 24 06:35:41 2013
  author    robok0der
*/

namespace NsAppLinkRPCV2
{
    class ReadDID_response : public NsAppLinkRPC::ALRPCResponse
    {
    public:
        ReadDID_response(const ReadDID_response& c);
        ReadDID_response(void);

        virtual ~ReadDID_response(void);

        ReadDID_response& operator =(const ReadDID_response&);

        bool checkIntegrity(void);

        const std::vector<VehicleDataResultCode>* get_dataResult(void) const;
        const std::vector<std::string>* get_data(void) const;

        void reset_dataResult(void);
        bool set_dataResult(const std::vector<VehicleDataResultCode>& dataResult_);
        void reset_data(void);
        bool set_data(const std::vector<std::string>& data_);

    private:
        friend class ReadDID_responseMarshaller;

        ///  Permission status of data element.
        std::vector<VehicleDataResultCode>* dataResult;   //!<   [%s..%s]

        ///  Raw DID-based data returned for requested element.
        std::vector<std::string>* data;   //!<   [%s..%s] (5000)
    };
}

#endif
