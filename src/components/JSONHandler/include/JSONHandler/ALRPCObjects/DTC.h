#ifndef NSAPPLINKRPC_DTC_INCLUDE
#define NSAPPLINKRPC_DTC_INCLUDE

#include <string>



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
