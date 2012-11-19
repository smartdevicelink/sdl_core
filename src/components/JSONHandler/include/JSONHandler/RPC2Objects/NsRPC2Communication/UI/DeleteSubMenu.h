#ifndef NSRPC2COMMUNICATION_UI_DELETESUBMENU_INCLUDE
#define NSRPC2COMMUNICATION_UI_DELETESUBMENU_INCLUDE

#include "JSONHandler/RPC2Request.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Mon Nov 19 07:25:04 2012
  source stamp	Mon Nov 19 07:04:40 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class DeleteSubMenu : public ::RPC2Communication::RPC2Request
    {
    public:
    
      DeleteSubMenu(const DeleteSubMenu& c);
      DeleteSubMenu(void);
    
      DeleteSubMenu& operator =(const DeleteSubMenu&);
    
      virtual ~DeleteSubMenu(void);
    
      bool checkIntegrity(void);
    
// getters
      unsigned int get_menuId(void);


// setters
/// menuId <= 2000000000
      bool set_menuId(unsigned int menuId);


    private:

      friend class DeleteSubMenuMarshaller;

      unsigned int menuId;

    };
  }
}

#endif
