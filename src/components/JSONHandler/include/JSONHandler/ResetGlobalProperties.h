#ifndef RESETGLOBAL_PROPERTIES_CLASS
#define RESETGLOBAL_PROPERTIES_CLASS

#include "JSONHandler/RPC2Request.h"
#include "JSONHandler/ALRPCObjects/GlobalProperty.h"
#include <vector>

namespace RPC2Communication 
{
    class ResetGlobalProperties : public RPC2Request
    {
    public:
        ResetGlobalProperties();
        ~ResetGlobalProperties();

        void setProperty(const std::vector<GlobalProperty>& property);
        const std::vector<GlobalProperty>& getProperty() const;
    
    private:
        std::vector<GlobalProperty> mProperty;
        friend class ResetGlobalPropertiesMarshaller;
    };
}

#endif