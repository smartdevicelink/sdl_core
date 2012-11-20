#ifndef NSRPC2COMMUNICATION_UI_ADDSUBMENURESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_UI_ADDSUBMENURESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Nov 20 13:32:23 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class AddSubMenuResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      AddSubMenuResponse(const AddSubMenuResponse& c);
      AddSubMenuResponse(void);
    
      AddSubMenuResponse& operator =(const AddSubMenuResponse&);
    
      virtual ~AddSubMenuResponse(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class AddSubMenuResponseMarshaller;


    };
  }
}

#endif
