#ifndef DELETESUBMENU_CLASS
#define DELETESUBMENU_CLASS

#include "JSONHandler/RPC2Request.h"

namespace RPC2Communication
{
    class DeleteSubMenu : public RPC2Request
    {
    public:
        DeleteSubMenu();
        ~DeleteSubMenu();

        void setMenuId(unsigned int menuId);
        unsigned int getMenuId() const;
    
    private:
        unsigned int menuId;
        friend class DeleteSubMenuMarshaller;
    };
}

#endif