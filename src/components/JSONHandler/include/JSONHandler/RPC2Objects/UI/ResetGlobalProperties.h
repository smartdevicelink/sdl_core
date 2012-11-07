#ifndef RPC2COMMUNICATION_UI_RESETGLOBALPROPERTIES_INCLUDE
#define RPC2COMMUNICATION_UI_RESETGLOBALPROPERTIES_INCLUDE

#include <vector>
#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/GlobalProperty.h"

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

    class ResetGlobalProperties : public ::RPC2Communication::RPC2Request
    {
    public:
    
      ResetGlobalProperties(const ResetGlobalProperties& c);
      ResetGlobalProperties(void);
    
      ResetGlobalProperties& operator =(const ResetGlobalProperties&);
    
      virtual ~ResetGlobalProperties(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::vector< ::AppLinkRPC::GlobalProperty>& get_properties(void);


// setters
/// 1 <= size <= 100
      bool set_properties(const std::vector< ::AppLinkRPC::GlobalProperty>& properties);


    private:

      friend class ResetGlobalPropertiesMarshaller;

      std::vector< ::AppLinkRPC::GlobalProperty> properties;

    };
  }
}

#endif
