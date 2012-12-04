#ifndef NSRPC2COMMUNICATION_UI_DELETESUBMENU_INCLUDE
#define NSRPC2COMMUNICATION_UI_DELETESUBMENU_INCLUDE

#include "JSONHandler/RPC2Request.h"


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

    class DeleteSubMenu : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      DeleteSubMenu(const DeleteSubMenu& c);
      DeleteSubMenu(void);
    
      DeleteSubMenu& operator =(const DeleteSubMenu&);
    
      virtual ~DeleteSubMenu(void);
    
      bool checkIntegrity(void);
    
// getters
      unsigned int get_menuId(void);

      int get_appId(void);


// setters
/// menuId <= 2000000000
      bool set_menuId(unsigned int menuId);

      bool set_appId(int appId);


    private:

      friend class DeleteSubMenuMarshaller;

      unsigned int menuId;
      int appId;

    };
  }
}

#endif
