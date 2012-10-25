#ifndef DELETESUBMENU_REQUEST_INCLUDE
#define DELETESUBMENU_REQUEST_INCLUDE


#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
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
