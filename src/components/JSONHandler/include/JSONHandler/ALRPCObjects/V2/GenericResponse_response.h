#ifndef NSAPPLINKRPCV2_GENERICRESPONSE_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_GENERICRESPONSE_RESPONSE_INCLUDE

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
    /**
         Generic Response is sent, when the name of a received msg cannot be retrieved. Only used in case of an error.
         Currently, only resultCode INVALID_DATA is used.
    */
    class GenericResponse_response : public NsAppLinkRPC::ALRPCResponse
    {
    public:
        GenericResponse_response(const GenericResponse_response& c);
        GenericResponse_response(void);

        virtual ~GenericResponse_response(void);

        GenericResponse_response& operator =(const GenericResponse_response&);

        bool checkIntegrity(void);

    private:
        friend class GenericResponse_responseMarshaller;
    };

}

#endif
