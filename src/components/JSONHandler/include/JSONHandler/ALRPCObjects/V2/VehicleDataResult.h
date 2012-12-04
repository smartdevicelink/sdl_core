#ifndef NSAPPLINKRPCV2_VEHICLEDATARESULT_INCLUDE
#define NSAPPLINKRPCV2_VEHICLEDATARESULT_INCLUDE


#include "VehicleDataType.h"
#include "VehicleDataResultCode.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/


namespace NsAppLinkRPCV2
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
