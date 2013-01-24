#ifndef NSAPPLINKRPC_TEXTFIELD_INCLUDE
#define NSAPPLINKRPC_TEXTFIELD_INCLUDE


#include "CharacterSet.h"
#include "TextFieldName.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	robok0der
*/


namespace NsAppLinkRPC
{

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


///  The character set that is supported in this field. See CharacterSet.
      CharacterSet characterSet;

///  The name that identifies the field. See TextFieldName.
      TextFieldName name;

///  The number of rows of this field.
      unsigned int rows;	//!<  (1,3)

///  The number of characters in one row of this field.
      unsigned int width;	//!<  (1,40)
  };

}

#endif
