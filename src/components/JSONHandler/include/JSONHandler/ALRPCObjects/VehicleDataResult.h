#ifndef NSAPPLINKRPC_VEHICLEDATARESULT_INCLUDE
#define NSAPPLINKRPC_VEHICLEDATARESULT_INCLUDE


#include "VehicleDataType.h"
#include "VehicleDataResultCode.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/


namespace NsAppLinkRPC
{

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


///  Defined published data element type.
      VehicleDataType dataType;

///  Published data result code.
      VehicleDataResultCode resultCode;
  };

}

#endif
