#ifndef NSAPPLINKRPC_ADDSUBMENU_REQUEST_INCLUDE
#define NSAPPLINKRPC_ADDSUBMENU_REQUEST_INCLUDE

#include <string>

#include "JSONHandler/SDLRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	robok0der
*/

namespace NsSmartDeviceLinkRPC
{

///  Adds a sub menu to the in-application menu.

  class AddSubMenu_request : public SDLRPCRequest
  {
  public:
  
    AddSubMenu_request(const AddSubMenu_request& c);
    AddSubMenu_request(void);
    
    virtual ~AddSubMenu_request(void);
  
    AddSubMenu_request& operator =(const AddSubMenu_request&);
  
    bool checkIntegrity(void);

    unsigned int get_menuID(void) const;
    const unsigned int* get_position(void) const;
    const std::string& get_menuName(void) const;

    bool set_menuID(unsigned int menuID_);
    void reset_position(void);
    bool set_position(unsigned int position_);
    bool set_menuName(const std::string& menuName_);

  private:
  
    friend class AddSubMenu_requestMarshaller;


///  unique ID of the sub menu to add.
      unsigned int menuID;	//!<  (0,2000000000)

/**
     Position within the items that are are at top level of the in application menu.
     0 will insert at the front.
     1 will insert at the second position.
     If position is greater or equal than the number of items on top level, the sub menu will be appended to the end.
     Position of any submenu will always be located before the return and exit options
     If this param was omitted the entry will be added at the end.
*/
      unsigned int* position;	//!<  (0,1000)

///  Text to show in the menu for this sub menu.
      std::string menuName;	//!< (500)
  };

}

#endif
