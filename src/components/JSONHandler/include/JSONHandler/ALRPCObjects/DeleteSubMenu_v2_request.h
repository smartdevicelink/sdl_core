#ifndef NSAPPLINKRPC_DELETESUBMENU_V2_REQUEST_INCLUDE
#define NSAPPLINKRPC_DELETESUBMENU_V2_REQUEST_INCLUDE


#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

///  Deletes a submenu from the in-application menu.

  class DeleteSubMenu_v2_request : public ALRPCRequest
  {
  public:
  
    DeleteSubMenu_v2_request(const DeleteSubMenu_v2_request& c);
    DeleteSubMenu_v2_request(void);
    
    virtual ~DeleteSubMenu_v2_request(void);
  
    bool checkIntegrity(void);

    unsigned int get_menuID(void) const;

    bool set_menuID(unsigned int menuID_);

  private:
  
    friend class DeleteSubMenu_v2_requestMarshaller;


///  The "menuID" of the submenu to delete. (See addSubMenu.menuID)
      unsigned int menuID;	//!<  (0,2000000000)
  };

}

#endif
