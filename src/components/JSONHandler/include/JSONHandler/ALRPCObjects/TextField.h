#ifndef TEXTFIELD_INCLUDE
#define TEXTFIELD_INCLUDE


#include "CharacterSet.h"
#include "TextFieldName.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
  author	robok0der
*/



class TextField
{
public:

  TextField(const TextField& c);
  TextField(void);

  bool checkIntegrity(void);
// getters

  const CharacterSet& get_characterSet(void) const;
  const TextFieldName& get_name(void) const;
  unsigned int get_rows(void) const;
  unsigned int get_width(void) const;

// setters

  bool set_characterSet(const CharacterSet& characterSet_);
  bool set_name(const TextFieldName& name_);
  bool set_rows(unsigned int rows_);
  bool set_width(unsigned int width_);

private:

  friend class TextFieldMarshaller;

  CharacterSet characterSet;
  TextFieldName name;
  unsigned int rows;	//!<  (1,3)
  unsigned int width;	//!<  (1,40)
};

#endif
