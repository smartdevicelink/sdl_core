#ifndef CHOICE_INCLUDE
#define CHOICE_INCLUDE

#include <string>
#include <vector>



/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
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
  const std::string& get_menuName(void) const;
  const std::vector<std::string>& get_vrCommands(void) const;

// setters

  bool set_choiceID(unsigned int choiceID_);
  bool set_menuName(const std::string& menuName_);
  bool set_vrCommands(const std::vector<std::string>& vrCommands_);

private:

  friend class ChoiceMarshaller;

  unsigned int choiceID;	//!<  (0,65535)
  std::string menuName;	//!< (500)
  std::vector<std::string> vrCommands;	//!<   [%s..%s] (99)
};

#endif
