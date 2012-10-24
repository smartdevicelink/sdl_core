#ifndef SOFTBUTTON_INCLUDE
#define SOFTBUTTON_INCLUDE

#include <string>

#include "Image.h"
#include "SystemAction.h"
#include "SoftButtonType.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/



class SoftButton
{
public:

  SoftButton(const SoftButton& c);
  SoftButton(void);

  bool checkIntegrity(void);
// getters

  const Image& get_image(void) const;
  bool get_isHighlighted(void) const;
  unsigned int get_softButtonID(void) const;
  const SystemAction& get_systemAction(void) const;
  const std::string& get_text(void) const;
  const SoftButtonType& get_type(void) const;

// setters

  bool set_image(const Image& image_);
  bool set_isHighlighted(bool isHighlighted_);
  bool set_softButtonID(unsigned int softButtonID_);
  bool set_systemAction(const SystemAction& systemAction_);
  bool set_text(const std::string& text_);
  bool set_type(const SoftButtonType& type_);

private:

  friend class SoftButtonMarshaller;

  Image image;
  bool isHighlighted;
  unsigned int softButtonID;	//!<  (0,65536)
  SystemAction systemAction;
  std::string text;	//!< (500)
  SoftButtonType type;
};

#endif
