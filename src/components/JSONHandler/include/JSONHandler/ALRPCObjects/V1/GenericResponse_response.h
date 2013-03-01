#ifndef NSAPPLINKRPC_GENERICRESPONSE_RESPONSE_INCLUDE
#define NSAPPLINKRPC_GENERICRESPONSE_RESPONSE_INCLUDE

#include "JSONHandler/ALRPCResponse.h"

/*
  interface Ford Sync RAPI
  version   1.2
  date      2011-05-17
  generated at  Thu Jan 24 06:36:21 2013
  source stamp  Thu Jan 24 06:35:34 2013
  author    robok0der
*/

namespace NsAppLinkRPC
{
    /**
         Generic Response is sent, when the name of a received msg cannot be retrieved. Only used in case of an error.
         Currently, only resultCode INVALID_DATA is used.
    */
    class GenericResponse_response : public ALRPCResponse
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
