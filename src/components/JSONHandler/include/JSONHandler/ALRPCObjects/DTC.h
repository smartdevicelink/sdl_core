#ifndef DTC_INCLUDE
#define DTC_INCLUDE

#include <string>



/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/



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

  std::string identifier;	//!< (500)
  std::string statusByte;	//!< (500)
};

#endif
