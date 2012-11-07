#include "JSONHandler/DeleteSubMenu.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

DeleteSubMenu::DeleteSubMenu()
:RPC2Request( RPC2Marshaller :: METHOD_DELETESUBMENU_REQUEST )
{}
 
DeleteSubMenu::~DeleteSubMenu()
{}

void DeleteSubMenu::setMenuId(unsigned int menuId)
{
    this->menuId = menuId;
}
        
unsigned int DeleteSubMenu::getMenuId() const
{
    return menuId;
}
