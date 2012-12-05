#ifndef NSAPPLINKRPC_DELETESUBMENU_REQUEST_INCLUDE
#define NSAPPLINKRPC_DELETESUBMENU_REQUEST_INCLUDE


#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 16:02:39 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

///  Deletes a submenu from the in-application menu.

  class DeleteSubMenu_request : public ALRPCRequest
  {
  public:
  
    DeleteSubMenu_request(const DeleteSubMenu_request& c);
    DeleteSubMenu_request(void);
    
    virtual ~DeleteSubMenu_request(void);
  
    bool checkIntegrity(void);

    unsigned int get_menuID(void) const;

    bool set_menuID(unsigned int menuID_);

  private:
  
    friend class DeleteSubMenu_requestMarshaller;


///  The "menuID" of the submenu to delete. (See addSubMenu.menuID)
      unsigned int menuID;	//!<  (0,2000000000)
  };

}

#endif
