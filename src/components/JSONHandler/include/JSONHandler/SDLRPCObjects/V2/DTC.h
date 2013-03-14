#ifndef NSAPPLINKRPCV2_DTC_INCLUDE
#define NSAPPLINKRPCV2_DTC_INCLUDE

#include <string>



/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/


namespace NsSmartDeviceLinkRPCV2
{

  class DTC
  {
  public:
  
    DTC(const DTC& c);
    DTC(void);
  
    bool checkIntegrity(void);
  // getters

    const std::string& get_identifier(void) const;
    const std::string& get_statusByte(void) const;

// setters

    bool set_identifier(const std::string& identifier_);
    bool set_statusByte(const std::string& statusByte_);

  private:

    friend class DTCMarshaller;


///  String containing hexadecimal identifier as well as other common names.
      std::string identifier;	//!< (500)

///  Hexadecimal byte string.
      std::string statusByte;	//!< (500)
  };

}

#endif
