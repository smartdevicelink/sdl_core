#include "SmartObjects/CSmartObject.hpp"
#include <limits>
#include <errno.h>



NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(void)
{
    m_type = SmartType_Null;
    m_data.str_value = NULL;
}


NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(int i)
{
    set_value_integer(i);
}

NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(long l)
{
    set_value_integer(l);
}

NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(char c)
{
    set_value_char(c);
}

NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(bool b)
{
    set_value_bool(b);
}

NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(double d)
{
    set_value_double(d);
}

NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(const std::string s)
{
    set_value_string(s);
}

NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(char* s)
{
    set_value_cstr(s);
    return;
}

void NsAppLink::NsSmartObjects::CSmartObject::duplicate(const NsAppLink::NsSmartObjects::CSmartObject& object)
{
    m_type = object.m_type;

    switch(m_type) {
    case SmartType_Map :
        *m_data.map_value = *object.m_data.map_value;
        break;
    case SmartType_Array :
        *m_data.array_value = *object.m_data.array_value;
        break;
    case SmartType_Integer:
        m_data.long_value = object.m_data.long_value;
        break;
    case SmartType_Double :
        m_data.double_value = object.m_data.double_value;
        break;
    case SmartType_Boolean :
        m_data.bool_value = object.m_data.bool_value;
        break;
    case SmartType_Character :
        m_data.char_value = object.m_data.char_value;
        break;
    case SmartType_String :
        *m_data.str_value = *object.m_data.str_value;
        break;
//    default : ;
    }
}

  NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(const NsAppLink::NsSmartObjects::CSmartObject& object)
  {
    duplicate(object);
  }

NsAppLink::NsSmartObjects::CSmartObject::~CSmartObject()
{
    cleanup_data();
}

void NsAppLink::NsSmartObjects::CSmartObject::cleanup_data()
{
    switch(m_type) {
    case SmartType_String :
        delete m_data.str_value;
        break;
    case SmartType_Map:
        delete m_data.map_value;
        break;
    case SmartType_Array :
        delete m_data.array_value;
        break;
//    default : ;
    }
}

size_t NsAppLink::NsSmartObjects::CSmartObject::size()
{
    size_t size = 0;

    switch(m_type)
    {
    case SmartType_String:
        size = m_data.str_value->size();
        break;
    case SmartType_Array:
        size = m_data.array_value->size();
        break;
    case SmartType_Map:
        size = m_data.map_value->size();
        break;
    case SmartType_Boolean:
    case SmartType_Character:
    case SmartType_Double:
    case SmartType_Integer:
        // TODO: Discuss what is best to return for single value objects
        size = 1;
        break;
    default:
        size = 0;
        break;
    }

    return size;
}


void NsAppLink::NsSmartObjects::CSmartObject::cleanup_data_if_type_changed_and_set_new_type(SmartType newType)
{
    if(m_type != newType)
    {
        cleanup_data();
        m_type = newType;
    }
}

NsAppLink::NsSmartObjects::CSmartObject::operator int(void) const
{
    return convert_int();
}

NsAppLink::NsSmartObjects::CSmartObject::operator double(void) const
{
    return convert_double();
}

NsAppLink::NsSmartObjects::CSmartObject::operator char(void) const
{
    return convert_char();
}

NsAppLink::NsSmartObjects::CSmartObject::operator bool(void) const
{
    return convert_bool();
}

NsAppLink::NsSmartObjects::CSmartObject::operator long(void) const
{
    return convert_long();
}

NsAppLink::NsSmartObjects::CSmartObject::operator std::string(void) const
{
    return convert_string();
}


int NsAppLink::NsSmartObjects::CSmartObject::convert_int(void) const
{
    long retval;

    switch (m_type) {
    case SmartType_Integer:
        retval = m_data.long_value;
        break;
    case SmartType_Boolean:
        if (m_data.bool_value == true) return 1;
        else return 0;
    case SmartType_Double :
        retval = static_cast<long>(m_data.double_value);
        break;
    case SmartType_Character :
        return static_cast<long>(m_data.char_value);
    case SmartType_String :
        retval = convert_string_to_long(m_data.str_value);
        break;
    case SmartType_Null:
        return 0;
    case SmartType_Map :
    case SmartType_Array :
        return invalid_int_value;
    default :
        return invalid_int_value;
    }

    if (retval > std::numeric_limits<int>::max() ||
        retval < std::numeric_limits<int>::min())
        return invalid_int_value;
    else return static_cast<int>(m_data.long_value);

    return retval;
}

long NsAppLink::NsSmartObjects::CSmartObject::convert_long(void) const
{
    switch (m_type) {
    case SmartType_Integer :
        return m_data.long_value;
    case SmartType_Boolean :
        if (m_data.bool_value) return 1;
        else return 0;
    case SmartType_Double :
        return static_cast<int>(m_data.double_value);
    case SmartType_Character :
        return static_cast<char>(m_data.char_value);
    case SmartType_String :
        return convert_string_to_long(m_data.str_value);
    case SmartType_Map :
    case SmartType_Array :
        return invalid_int_value;
    case SmartType_Null :
        return 0;
    default :
        return invalid_int_value;
    }

    return 0L;
}

std::string NsAppLink::NsSmartObjects::CSmartObject::convert_string(void) const
{
    char buf[32];

    switch (m_type) {
    case SmartType_String :
        return *(m_data.str_value);
    case SmartType_Integer :
        snprintf(buf,32,"%ld",m_data.long_value);
        return std::string(buf);
        break;
    case SmartType_Double :
        snprintf(buf,32,"%g",m_data.double_value);
        return std::string(buf);
        break;
    case SmartType_Boolean :
        if (m_data.bool_value) return std::string(true_str);
        else return std::string(false_str);
    case SmartType_Character :
        snprintf(buf,32,"%c",m_data.char_value);
        return(buf);
        break;
    case SmartType_Null :
        return std::string("null");
        break;
    case SmartType_Map :
    case SmartType_Array :
        return invalid_string_value;
    default :
        return invalid_string_value;
    }

    return std::string("error");
}

char NsAppLink::NsSmartObjects::CSmartObject::convert_char(void) const
{
    switch (m_type) {
    case SmartType_Character :
        return m_data.char_value;
    case SmartType_Boolean :
        if (m_data.bool_value) return 't';
        else return 'f';
    case SmartType_String :
        return m_data.str_value->c_str()[0];
    case SmartType_Integer :
    case SmartType_Double :
        return invalid_char_value;
    case SmartType_Map :
    case SmartType_Array :
        return invalid_char_value;
    case SmartType_Null :
        return '\0';
    default :
        return invalid_char_value;
    }

    return '\0';
}

bool NsAppLink::NsSmartObjects::CSmartObject::convert_bool(void) const
{
    switch (m_type) {
    case SmartType_Boolean :
        return m_data.bool_value;
    case SmartType_Character :
        if (m_data.char_value != 0) return true;
        else return false;
    case SmartType_Integer :
        return (m_data.long_value != 0);
        break;
    case SmartType_Double :
        return (m_data.double_value != 0.0);
        break;
    case SmartType_String :
        if (!m_data.str_value->compare(true_str)) return true;
        else if(!m_data.str_value->compare(false_str)) return false;
        return invalid_bool_value;
    case SmartType_Map :
    case SmartType_Array :
        return invalid_bool_value;
    case SmartType_Null :
        return false;
        break;
    default :
        return invalid_bool_value;
    }

    return false;
}

double NsAppLink::NsSmartObjects::CSmartObject::convert_double(void) const
{
    switch (m_type) {
    case SmartType_Double:
        return m_data.double_value;
    case SmartType_Integer:
        return static_cast<double>(m_data.long_value);
    case SmartType_Boolean:
        if (m_data.bool_value) return 1.0;
        else return 0.0;
    case SmartType_Character:
        return static_cast<double>(m_data.char_value);
    case SmartType_String:
        return convert_string_to_double(m_data.str_value);
    case SmartType_Map:
    case SmartType_Array:
        return invalid_double_value;
    case SmartType_Null:
        return 0.0;
    default:
        return invalid_double_value;
    }

    return 0.0;
}

double NsAppLink::NsSmartObjects::CSmartObject::convert_string_to_double(const std::string* s)
{
    double retval;
    char* ptr;
    errno = 0;

    retval = strtod(s->c_str(),&ptr);
    if (errno || (retval == 0.0 &&
        ptr != (s->c_str() + s->length())))
        return invalid_double_value;
    return retval;
}

long NsAppLink::NsSmartObjects::CSmartObject::convert_string_to_long(const std::string* s)
{
    errno = 0;
    long l = strtol(s->c_str(),NULL,10);
    if (errno) return invalid_int_value;
    return l;
}

void NsAppLink::NsSmartObjects::CSmartObject::set_value_integer(long l)
{
    cleanup_data_if_type_changed_and_set_new_type(SmartType_Integer);
    m_data.long_value = l;
}

void NsAppLink::NsSmartObjects::CSmartObject::set_value_char(char c)
{
    cleanup_data_if_type_changed_and_set_new_type(SmartType_Character);
    m_data.char_value = c;
}

void NsAppLink::NsSmartObjects::CSmartObject::set_value_double(double d)
{
    cleanup_data_if_type_changed_and_set_new_type(SmartType_Double);
    m_data.double_value = d;
}

void NsAppLink::NsSmartObjects::CSmartObject::set_value_bool(bool b)
{
    cleanup_data_if_type_changed_and_set_new_type(SmartType_Boolean);
    m_data.bool_value = b;
}

void NsAppLink::NsSmartObjects::CSmartObject::set_value_string(const std::string& s)
{
    cleanup_data_if_type_changed_and_set_new_type(SmartType_String);
    m_data.str_value = new std::string(s);
}

  void NsAppLink::NsSmartObjects::CSmartObject::set_value_cstr(const char* s)
  {
      if (s)
          set_value_string(std::string(s));
      else {
          std::string tmp;
          set_value_string(tmp);
      }
  }

  NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator[](const std::string s)
  {
    return handle_map_access(s);
  }

  NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator[](char* s)
  {
    std::string str(s);
    return handle_map_access(str);
  }


  NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator[](const char* s)
  {
    std::string str(s);
    return handle_map_access(str);
  }


  NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator[](int i)
  {
      return handle_array_access(i);
  }

  inline NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::handle_array_access(int index)
  {
      if(m_type != SmartType_Array)
      {
          cleanup_data();
          m_type = SmartType_Array;
          m_data.array_value = new SmartArray();
      }

      int sz = m_data.array_value->size();
      if (index == -1)
      {
          index = sz;
      }
      if (index == sz)
      {
          NsAppLink::NsSmartObjects::CSmartObject uc;
          m_data.array_value->push_back(uc);
      }
      if (index > sz || index < 0)
      {
          return invalid_object_value;
      }
      return m_data.array_value->at(index);
  }

  NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::handle_map_access(const std::string s)
  {
      if(m_type != SmartType_Map)
      {
          cleanup_data();
          m_type = SmartType_Map;
          m_data.map_value = new NsAppLink::NsSmartObjects::SmartMap();
      }

      //TODO: Add check for key presense
      return (*(m_data.map_value))[s];
  }



  NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator=(int i)
  {
      set_value_integer(i);
      return *this;
  }

  NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator=(long l)
  {
      set_value_integer(l);
      return *this;
  }

  NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator=(double d)
  {
      set_value_double(d);
      return *this;
  }

  NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator=(bool b)
  {
      set_value_bool(b);
      return *this;
  }

  NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator=(const std::string& s)
  {
      set_value_string(s);
      return *this;
  }

  NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator=(const char* s)
  {
      set_value_cstr(s);
      return *this;
  }

  NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator=(char c)
  {
      set_value_char(c);
      return *this;
  }

  NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator=(const NsAppLink::NsSmartObjects::CSmartObject& uc)
  {
      if (this != &uc) duplicate(uc);
      return *this;
  }

  NsAppLink::NsSmartObjects::SmartType NsAppLink::NsSmartObjects::CSmartObject::get_type()
  {
      return m_type;
  }


  bool NsAppLink::NsSmartObjects::CSmartObject::operator==(const NsAppLink::NsSmartObjects::CSmartObject& object) const
  {
      if (m_type != object.m_type) return false;

      switch(m_type) {
          case SmartType_Integer :
              return m_data.long_value == object.m_data.long_value;
          case SmartType_Double :
              return m_data.double_value == object.m_data.double_value;
          case SmartType_Boolean :
              return m_data.bool_value == object.m_data.bool_value;
          case SmartType_Character :
              return m_data.char_value == object.m_data.char_value;
          case SmartType_String :
              return *(m_data.str_value) == *(object.m_data.str_value);
          case SmartType_Map :
              if (m_data.map_value == object.m_data.map_value) return true;
              return std::equal(m_data.map_value->begin(), m_data.map_value->end(), object.m_data.map_value->begin());
          case SmartType_Array :
              if (m_data.array_value == object.m_data.array_value) return true;
              return std::equal(m_data.array_value->begin(), m_data.array_value->end(), object.m_data.array_value->begin());
          case SmartType_Null :
              return true;
      }
      return false;
  }

  //equivalance tests for primitives.
  bool NsAppLink::NsSmartObjects::CSmartObject::operator==(int val) const
  {
      int comp = convert_int();
      if(comp == NsAppLink::NsSmartObjects::invalid_int_value)
      {
          return false;
      }
      else
      {
          return comp == val;
      }
  }

  bool NsAppLink::NsSmartObjects::CSmartObject::operator==(long val) const
  {
      int comp = convert_long();
      if(comp == NsAppLink::NsSmartObjects::invalid_int_value)
      {
          return false;
      }
      else
      {
          return comp == val;
      }
  }

  bool NsAppLink::NsSmartObjects::CSmartObject::operator==(std::string val) const
  {
      std::string comp = convert_string();
      if(comp == NsAppLink::NsSmartObjects::invalid_string_value)
      {
          return false;
      }
      else
      {
          return comp == val;
      }
  }

  bool NsAppLink::NsSmartObjects::CSmartObject::operator==(double val) const
  {
      double comp = convert_double();
      if(comp == NsAppLink::NsSmartObjects::invalid_double_value)
      {
          return false;
      }
      else
      {
          return comp == val;
      }
  }

  bool NsAppLink::NsSmartObjects::CSmartObject::operator==(bool val) const
  {
      bool comp = convert_bool();
      if(comp == NsAppLink::NsSmartObjects::invalid_bool_value)
      {
          return false;
      }
      else
      {
          return comp == val;
      }
  }

  bool NsAppLink::NsSmartObjects::CSmartObject::operator==(char val) const
  {
      char comp = convert_char();
      if(comp == NsAppLink::NsSmartObjects::invalid_char_value)
      {
          return false;
      }
      else
      {
          return comp == val;
      }
  }