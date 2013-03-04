#ifndef NSAPPLINKRPCV2_ADDSUBMENU_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_ADDSUBMENU_RESPONSE_INCLUDE

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
    class AddSubMenu_response : public NsAppLinkRPC::ALRPCResponse
    {
    public:
        AddSubMenu_response(const AddSubMenu_response& c);
        AddSubMenu_response(void);

        virtual ~AddSubMenu_response(void);

        AddSubMenu_response& operator =(const AddSubMenu_response&);

        bool checkIntegrity(void);
    private:
        friend class AddSubMenu_responseMarshaller;
    };
}

#endif
