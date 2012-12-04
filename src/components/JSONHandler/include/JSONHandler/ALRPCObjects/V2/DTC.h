#ifndef NSAPPLINKRPCV2_DTC_INCLUDE
#define NSAPPLINKRPCV2_DTC_INCLUDE

#include <string>



/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/


namespace NsAppLinkRPCV2
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
