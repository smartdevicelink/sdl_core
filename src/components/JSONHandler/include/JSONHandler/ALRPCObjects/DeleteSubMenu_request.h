#ifndef DELETESUBMENU_REQUEST_INCLUDE
#define DELETESUBMENU_REQUEST_INCLUDE


#include "../../../JSONHandler/ALRPCRequest.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


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

  unsigned int menuID;	//!<  (0,2000000000)
};

#endif
