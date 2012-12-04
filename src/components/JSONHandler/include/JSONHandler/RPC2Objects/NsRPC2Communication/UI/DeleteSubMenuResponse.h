#ifndef NSRPC2COMMUNICATION_UI_DELETESUBMENURESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_UI_DELETESUBMENURESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Dec  4 15:06:30 2012
  source stamp	Tue Dec  4 14:21:32 2012
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
