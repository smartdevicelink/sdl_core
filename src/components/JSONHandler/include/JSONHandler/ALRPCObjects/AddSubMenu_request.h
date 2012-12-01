#ifndef NSAPPLINKRPC_ADDSUBMENU_REQUEST_INCLUDE
#define NSAPPLINKRPC_ADDSUBMENU_REQUEST_INCLUDE

#include <string>

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

///  Adds a sub menu to the in-application menu.

  class AddSubMenu_request : public ALRPCRequest
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
