#ifndef RPC2COMMUNICATION_UI_DELETESUBMENU_INCLUDE
#define RPC2COMMUNICATION_UI_DELETESUBMENU_INCLUDE

#include "JSONHandler/RPC2Request.h"


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
