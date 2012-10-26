#ifndef RESETGLOBAL_PROPERTIES_CLASS
#define RESETGLOBAL_PROPERTIES_CLASS

#include "JSONHandler/RPC2Request.h"
#include "JSONHandler/ALRPCObjects/GlobalProperty.h"

namespace RPC2Communication 
{
    class ResetGlobalProperties : public RPC2Request
    {
    public:
        ResetGlobalProperties();
        ~ResetGlobalProperties();

        void setProperty(const GlobalProperty& property);
        const GlobalProperty& getProperty() const;
    
    private:
        GlobalProperty mProperty;
        friend class ResetGlobalPropertiesMarshaller;
    };
}

#endif