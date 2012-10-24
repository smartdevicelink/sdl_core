#ifndef TURN_INCLUDE
#define TURN_INCLUDE

#include <string>

#include "Image.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/



class Turn
{
public:

  Turn(const Turn& c);
  Turn(void);

  bool checkIntegrity(void);

  ~Turn(void);
  Turn& operator =(const Turn&);

// getters

  const std::string* get_navigationText(void) const;
  const Image& get_turnIcon(void) const;

// setters

  void reset_navigationText(void);
  bool set_navigationText(const std::string& navigationText_);
  bool set_turnIcon(const Image& turnIcon_);

private:

  friend class TurnMarshaller;

  std::string* navigationText;	//!< (500)
  Image turnIcon;
};

#endif
