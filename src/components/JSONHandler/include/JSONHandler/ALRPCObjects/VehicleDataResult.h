#ifndef VEHICLEDATARESULT_INCLUDE
#define VEHICLEDATARESULT_INCLUDE


#include "VehicleDataType.h"
#include "VehicleDataResultCode.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/



///  Individual published data request result

class VehicleDataResult
{
public:

  VehicleDataResult(const VehicleDataResult& c);
  VehicleDataResult(void);

  bool checkIntegrity(void);
// getters

  const VehicleDataType& get_dataType(void) const;
  const VehicleDataResultCode& get_resultCode(void) const;

// setters

  bool set_dataType(const VehicleDataType& dataType_);
  bool set_resultCode(const VehicleDataResultCode& resultCode_);

private:

  friend class VehicleDataResultMarshaller;

  VehicleDataType dataType;
  VehicleDataResultCode resultCode;
};

#endif
