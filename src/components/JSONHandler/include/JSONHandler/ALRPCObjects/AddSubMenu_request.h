#ifndef ADDSUBMENU_REQUEST_INCLUDE
#define ADDSUBMENU_REQUEST_INCLUDE

#include <string>

#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/


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

  unsigned int menuID;	//!<  (0,2000000000)
  unsigned int* position;	//!<  (0,1000)
  std::string menuName;	//!< (500)
};

#endif
