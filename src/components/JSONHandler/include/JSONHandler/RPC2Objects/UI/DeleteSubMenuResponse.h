#ifndef RPC2COMMUNICATION_UI_DELETESUBMENURESPONSE_INCLUDE
#define RPC2COMMUNICATION_UI_DELETESUBMENURESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"


/*
  interface	RPC2Communication::UI
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

namespace RPC2Communication
{
  namespace UI
  {

    class DeleteSubMenuResponse : public ::RPC2Communication::RPC2Response
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
