#ifndef NSAPPLINKRPC_CHOICE_INCLUDE
#define NSAPPLINKRPC_CHOICE_INCLUDE

#include <string>
#include <vector>



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

}

#endif
