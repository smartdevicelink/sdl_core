#ifndef VRHELPITEM_INCLUDE
#define VRHELPITEM_INCLUDE

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



class VrHelpItem
{
public:

  VrHelpItem(const VrHelpItem& c);
  VrHelpItem(void);

  bool checkIntegrity(void);
// getters

  const Image& get_image(void) const;
  unsigned int get_position(void) const;
  const std::string& get_text(void) const;

// setters

  bool set_image(const Image& image_);
  bool set_position(unsigned int position_);
  bool set_text(const std::string& text_);

private:

  friend class VrHelpItemMarshaller;

  Image image;
  unsigned int position;	//!<  (1,8)
  std::string text;	//!< (500)
};

#endif
