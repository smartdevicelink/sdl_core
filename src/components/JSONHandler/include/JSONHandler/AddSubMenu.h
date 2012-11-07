#ifndef ADDSUBMENU_REQUEST_CLASS
#define ADDSUBMENU_REQUEST_CLASS


#include "JSONHandler/RPC2Request.h"
#include <string>

namespace RPC2Communication
{
    class AddSubMenu : public RPC2Request
    {
    public:
        AddSubMenu();
        ~AddSubMenu();

        int getMenuId() const;
        int * getPosition() const;
        const std::string & getMenuName() const;

        void setMenuId( int menuId );
        void setPosition( int position );
        void setMenuName( const std::string & menuName );

    private:
        int menuId;
        int * position;
        std::string menuName;
        friend class AddSubMenuMarshaller;
    };
}


#endif
