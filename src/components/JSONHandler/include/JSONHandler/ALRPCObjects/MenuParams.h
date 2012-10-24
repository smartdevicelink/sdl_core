#ifndef MENUPARAMS_INCLUDE
#define MENUPARAMS_INCLUDE

#include <string>



/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/



class MenuParams
{
public:

  MenuParams(const MenuParams& c);
  MenuParams(void);

  bool checkIntegrity(void);

  ~MenuParams(void);
  MenuParams& operator =(const MenuParams&);

// getters

  const std::string& get_menuName(void) const;
  const unsigned int* get_parentID(void) const;
  const unsigned int* get_position(void) const;

// setters

  bool set_menuName(const std::string& menuName_);
  void reset_parentID(void);
  bool set_parentID(unsigned int parentID_);
  void reset_position(void);
  bool set_position(unsigned int position_);

private:

  friend class MenuParamsMarshaller;

  std::string menuName;	//!< (500)
  unsigned int* parentID;	//!<  (0,2000000000)
  unsigned int* position;	//!<  (0,1000)
};

#endif
