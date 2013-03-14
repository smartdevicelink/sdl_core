#ifndef NSAPPLINKRPCV2_GETDTCS_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_GETDTCS_RESPONSE_INCLUDE

#include <vector>

#include "DTC.h"
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
    class GetDTCs_response : public NsSmartDeviceLinkRPC::SDLRPCResponse
    {
    public:
        GetDTCs_response(const GetDTCs_response& c);
        GetDTCs_response(void);

        virtual ~GetDTCs_response(void);

        GetDTCs_response& operator =(const GetDTCs_response&);

        bool checkIntegrity(void);

        const std::vector<DTC>* get_dtcList(void) const;
        void reset_dtcList(void);
        bool set_dtcList(const std::vector<DTC>& dtcList_);

    private:
        friend class GetDTCs_responseMarshaller;

        ///  Array of all reported DTCs on module.
        std::vector<DTC>* dtcList;    //!<   [%s..%s]
    };
}

#endif
