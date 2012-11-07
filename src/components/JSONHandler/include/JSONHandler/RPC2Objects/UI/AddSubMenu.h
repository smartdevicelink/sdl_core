#ifndef RPC2COMMUNICATION_UI_ADDSUBMENU_INCLUDE
#define RPC2COMMUNICATION_UI_ADDSUBMENU_INCLUDE

#include <string>
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

    class AddSubMenu : public ::RPC2Communication::RPC2Request
    {
    public:
    
      AddSubMenu(const AddSubMenu& c);
      AddSubMenu(void);
    
      AddSubMenu& operator =(const AddSubMenu&);
    
      virtual ~AddSubMenu(void);
    
      bool checkIntegrity(void);
    
// getters
      unsigned int get_menuId(void);

  const     unsigned int* get_position(void);
      const std::string& get_menuName(void);


// setters
/// menuId <= 2000000000
      bool set_menuId(unsigned int menuId);

/// position <= 1000
      bool set_position(const unsigned int& position);

      void reset_position(void);

/// menuName <= 500
      bool set_menuName(const std::string& menuName);


    private:

      friend class AddSubMenuMarshaller;

      unsigned int menuId;
      unsigned int* position;
      std::string menuName;

    };
  }
}

#endif
