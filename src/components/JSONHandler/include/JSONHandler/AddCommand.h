#ifndef ADDCOMMAND_REQUEST_CLASS
#define ADDCOMMAND_REQUEST_CLASS

#include "JSONHandler/RPC2Request.h"
#include "JSONHandler/ALRPCObjects/MenuParams.h"
#include <string>

namespace RPC2Communication
{
    class AddCommand : public RPC2Request
    {
    public:
        AddCommand();
        ~AddCommand();

        void setCmdId( int cmdId );
        void setMenuParams( const MenuParams& menuParams );

        int getCmdId() const;
        const MenuParams& getMenuParams() const;
    
    private:
        int cmdId;
        MenuParams menuParams;
        friend class AddCommandMarshaller;
    };
}


#endif