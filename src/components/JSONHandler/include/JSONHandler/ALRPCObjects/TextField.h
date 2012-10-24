#ifndef TEXTFIELD_INCLUDE
#define TEXTFIELD_INCLUDE


#include "CharacterSet.h"
#include "TextFieldName.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
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
  unsigned int width;	//!<  (1,500)
};

#endif
