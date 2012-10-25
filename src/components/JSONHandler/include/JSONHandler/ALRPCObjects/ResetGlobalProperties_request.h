#ifndef RESETGLOBALPROPERTIES_REQUEST_INCLUDE
#define RESETGLOBALPROPERTIES_REQUEST_INCLUDE

#include <vector>

#include "GlobalProperty.h"
#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


///  Allows resetting global properties.

class ResetGlobalProperties_request : public ALRPCRequest
{
public:

  ResetGlobalProperties_request(const ResetGlobalProperties_request& c);
  ResetGlobalProperties_request(void);
  
  virtual ~ResetGlobalProperties_request(void);

  bool checkIntegrity(void);

  const std::vector<GlobalProperty>& get_properties(void) const;

  bool set_properties(const std::vector<GlobalProperty>& properties_);

private:

  friend class ResetGlobalProperties_requestMarshaller;

  std::vector<GlobalProperty> properties;	//!<   [%s..%s] 
};

#endif
