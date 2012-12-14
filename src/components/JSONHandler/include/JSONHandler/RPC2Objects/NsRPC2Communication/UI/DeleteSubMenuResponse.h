#ifndef NSRPC2COMMUNICATION_UI_DELETESUBMENURESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_UI_DELETESUBMENURESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class DeleteSubMenuResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      DeleteSubMenuResponse(const DeleteSubMenuResponse& c);
      DeleteSubMenuResponse(void);
    
      DeleteSubMenuResponse& operator =(const DeleteSubMenuResponse&);
    
      virtual ~DeleteSubMenuResponse(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class DeleteSubMenuResponseMarshaller;


    };
  }
}

#endif
