#ifndef CHOICE_INCLUDE
#define CHOICE_INCLUDE

#include <string>
#include <vector>

#include "Image.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/



///  A choice is an option given to the user, which can be selected either by menu, or through voice recognition system.

class Choice
{
public:

  Choice(const Choice& c);
  Choice(void);

  bool checkIntegrity(void);
// getters

  unsigned int get_choiceID(void) const;
  const Image& get_image(void) const;
  const std::string& get_menuName(void) const;
  const std::vector<std::string>& get_vrCommands(void) const;

// setters

  bool set_choiceID(unsigned int choiceID_);
  bool set_image(const Image& image_);
  bool set_menuName(const std::string& menuName_);
  bool set_vrCommands(const std::vector<std::string>& vrCommands_);

private:

  friend class ChoiceMarshaller;

  unsigned int choiceID;	//!<  (0,65535)
  Image image;
  std::string menuName;	//!< (500)
  std::vector<std::string> vrCommands;	//!<   [%s..%s] (99)
};

#endif
