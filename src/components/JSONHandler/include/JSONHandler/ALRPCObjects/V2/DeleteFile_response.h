#ifndef NSAPPLINKRPCV2_DELETEFILE_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_DELETEFILE_RESPONSE_INCLUDE

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
         Response is sent, when the file data was deleted (success case). Or when an error occured.
         Not supported on First generation SYNC vehicles.
    */
    class DeleteFile_response : public NsAppLinkRPC::ALRPCResponse
    {
    public:
        DeleteFile_response(const DeleteFile_response& c);
        DeleteFile_response(void);

        virtual ~DeleteFile_response(void);

        DeleteFile_response& operator =(const DeleteFile_response&);

        bool checkIntegrity(void);

        unsigned int get_spaceAvailable(void) const;
        bool set_spaceAvailable(unsigned int spaceAvailable_);

    private:
        friend class DeleteFile_responseMarshaller;

        ///  Provides the total local space available on SYNC for the registered app.
        unsigned int spaceAvailable;  //!<  (0,2000000000)
    };
}

#endif
