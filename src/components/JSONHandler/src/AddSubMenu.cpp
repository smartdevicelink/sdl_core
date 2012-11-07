#include "JSONHandler/AddSubMenu.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

AddSubMenu::AddSubMenu()
:RPC2Request( RPC2Marshaller :: METHOD_ADDSUBMENU_REQUEST )
{
    position = 0;
}
 
AddSubMenu::~AddSubMenu()
{
    if(position)
    {
        delete position;
    }
}

int AddSubMenu::getMenuId() const
{
    return menuId;
}

int * AddSubMenu::getPosition() const
{
    return position;
}

const std::string &  AddSubMenu::getMenuName() const
{
    return menuName;
}

void AddSubMenu::setMenuId( int menuId )
{
    this->menuId = menuId;
}

void AddSubMenu::setPosition( int  position )
{
    if(this->position)
    {
        delete this->position;
    }
        
    this->position = new int(position);
}

void AddSubMenu::setMenuName( const std::string & menuName )
{
    this->menuName = menuName;
}
