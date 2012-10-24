#ifndef RESETGLOBALPROPERTIES_REQUEST_INCLUDE
#define RESETGLOBALPROPERTIES_REQUEST_INCLUDE

#include <vector>

#include "GlobalProperty.h"
#include "../../../JSONHandler/ALRPCRequest.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
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
