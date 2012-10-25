#ifndef MENUPARAMS_INCLUDE
#define MENUPARAMS_INCLUDE

#include <string>



/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
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
