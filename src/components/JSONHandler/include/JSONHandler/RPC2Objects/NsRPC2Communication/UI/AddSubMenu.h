#ifndef NSRPC2COMMUNICATION_UI_ADDSUBMENU_INCLUDE
#define NSRPC2COMMUNICATION_UI_ADDSUBMENU_INCLUDE

#include <string>
#include "JSONHandler/RPC2Request.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Mon Nov 19 10:37:39 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class AddSubMenu : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      AddSubMenu(const AddSubMenu& c);
      AddSubMenu(void);
    
      AddSubMenu& operator =(const AddSubMenu&);
    
      virtual ~AddSubMenu(void);
    
      bool checkIntegrity(void);
    
// getters
      unsigned int get_menuId(void);

      const unsigned int* get_position(void);
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
