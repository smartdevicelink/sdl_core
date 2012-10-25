#ifndef RESETGLOBALPROPERTIES_REQUEST_INCLUDE
#define RESETGLOBALPROPERTIES_REQUEST_INCLUDE

#include <vector>

#include "GlobalProperty.h"
#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
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
