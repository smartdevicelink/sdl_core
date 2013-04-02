#include "SmartObjects/CSmartObject.hpp"
#include <limits>
#include <errno.h>
#include <algorithm>
#include <sstream>
#include <iomanip>

NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(void)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
}

NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(const NsAppLink::NsSmartObjects::CSmartObject& Other)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
    duplicate(Other);
}

NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(SmartType Type)
: m_type(SmartType_Null)
, m_schema()
{
    switch(Type) {
        case SmartType_Integer :
            set_value_long(0);
            break;
        case SmartType_Double :
            set_value_double(0);
            break;
        case SmartType_Boolean :
            set_value_bool(false);
            break;
        case SmartType_Character :
            set_value_char(' ');
            break;
        case SmartType_String :
            set_value_string("");
            break;
        case SmartType_Binary :
            set_value_binary(SmartBinary());
            break;
        case SmartType_Invalid:
            m_type = SmartType_Invalid;
            break;
    }
}

NsAppLink::NsSmartObjects::CSmartObject::~CSmartObject()
{
    cleanup_data();
}

NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator=(const NsAppLink::NsSmartObjects::CSmartObject& Other)
{
    if (this != &Other) duplicate(Other);
    return *this;
}

bool NsAppLink::NsSmartObjects::CSmartObject::operator==(const NsAppLink::NsSmartObjects::CSmartObject& Other) const
{
    if (m_type != Other.m_type) return false;

    switch(m_type) {
        case SmartType_Integer :
            return m_data.long_value == Other.m_data.long_value;
        case SmartType_Double :
            return m_data.double_value == Other.m_data.double_value;
        case SmartType_Boolean :
            return m_data.bool_value == Other.m_data.bool_value;
        case SmartType_Character :
            return m_data.char_value == Other.m_data.char_value;
        case SmartType_String :
            return *(m_data.str_value) == *(Other.m_data.str_value);
        case SmartType_Map :
            if (m_data.map_value == Other.m_data.map_value) return true;
            return std::equal(m_data.map_value->begin(), m_data.map_value->end(), Other.m_data.map_value->begin());
        case SmartType_Array :
            if (m_data.array_value == Other.m_data.array_value) return true;
            return std::equal(m_data.array_value->begin(), m_data.array_value->end(), Other.m_data.array_value->begin());
        case SmartType_Binary :
            if (m_data.binary_value == Other.m_data.binary_value) return true;
            return std::equal(m_data.binary_value->begin(), m_data.binary_value->end(), Other.m_data.binary_value->begin());
        case SmartType_Null :
            return true;
        case SmartType_Invalid:
            return true;
    }
    return false;
}

// =============================================================
// INTEGER TYPE SUPPORT
// =============================================================
NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(int InitialValue)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
    set_value_integer(InitialValue);
}

NsAppLink::NsSmartObjects::CSmartObject::operator int(void) const
{
    return convert_int();
}

int NsAppLink::NsSmartObjects::CSmartObject::asInt()
{
    return (int)(*this);
}

NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator=(int NewValue)
{
    if(m_type != SmartType_Invalid)
    {
        set_value_integer(NewValue);
    }
    return *this;
}

bool NsAppLink::NsSmartObjects::CSmartObject::operator==(int Value) const
{
    int comp = convert_int();
    if(comp == NsAppLink::NsSmartObjects::invalid_int_value)
    {
        return false;
    }
    else
    {
        return comp == Value;
    }
}

void NsAppLink::NsSmartObjects::CSmartObject::set_value_integer(int NewValue)
{
    set_new_type(SmartType_Integer);
    m_data.long_value = NewValue;
}

int NsAppLink::NsSmartObjects::CSmartObject::convert_int(void) const
{
    long retval;

    switch (m_type) {
        case SmartType_String :
            retval = convert_string_to_long(m_data.str_value);
            break;
        case SmartType_Boolean:
            return (m_data.bool_value == true) ? 1 : 0;
        case SmartType_Integer:
            retval = m_data.long_value;
            break;
        case SmartType_Double :
            retval = static_cast<long>(m_data.double_value);
            break;
        default :
            return invalid_int_value;
    }

    if (retval > std::numeric_limits<int>::max() ||
        retval < std::numeric_limits<int>::min())
    {
        return invalid_int_value;
    }
    else
    {
        return static_cast<int>(retval);
    }
}

// =============================================================
// LONG TYPE SUPPORT
// =============================================================
NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(long InitialValue)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
    set_value_long(InitialValue);
}

NsAppLink::NsSmartObjects::CSmartObject::operator long(void) const
{
    return convert_long();
}

long NsAppLink::NsSmartObjects::CSmartObject::asLong()
{
    return (long)(*this);
}

NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator=(long NewValue)
{
    if(m_type != SmartType_Invalid)
    {
        set_value_integer(NewValue);
    }
    return *this;
}

bool NsAppLink::NsSmartObjects::CSmartObject::operator==(long Value) const
{
    int comp = convert_long();
    if(comp == NsAppLink::NsSmartObjects::invalid_int_value)
    {
        return false;
    }
    else
    {
        return comp == Value;
    }
}

void NsAppLink::NsSmartObjects::CSmartObject::set_value_long(long NewValue)
{
    set_new_type(SmartType_Integer);
    m_data.long_value = NewValue;
}

long NsAppLink::NsSmartObjects::CSmartObject::convert_long(void) const
{
    switch (m_type) {
        case SmartType_String :
            return convert_string_to_long(m_data.str_value);
            break;
        case SmartType_Boolean:
            return (m_data.bool_value == true) ? 1 : 0;
        case SmartType_Integer:
            return m_data.long_value;
            break;
        case SmartType_Double:
            return static_cast<long>(m_data.double_value);
            break;
        default :
            break;
    }

    return invalid_int_value;
}

// =============================================================
// DOUBLE TYPE SUPPORT
// =============================================================
NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(double InitialValue)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
    set_value_double(InitialValue);
}

NsAppLink::NsSmartObjects::CSmartObject::operator double(void) const
{
    return convert_double();
}

double NsAppLink::NsSmartObjects::CSmartObject::asDouble()
{
    return (double)(*this);
}

NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator=(double NewValue)
{
    if(m_type != SmartType_Invalid)
    {
        set_value_double(NewValue);
    }
    return *this;
}

bool NsAppLink::NsSmartObjects::CSmartObject::operator==(double Value) const
{
    double comp = convert_double();
    if(comp == NsAppLink::NsSmartObjects::invalid_double_value)
    {
        return false;
    }
    else
    {
        return comp == Value;
    }
}

void NsAppLink::NsSmartObjects::CSmartObject::set_value_double(double NewValue)
{
    set_new_type(SmartType_Double);
    m_data.double_value = NewValue;
}

double NsAppLink::NsSmartObjects::CSmartObject::convert_double(void) const
{
    switch (m_type) {
        case SmartType_String:
            return convert_string_to_double(m_data.str_value);
        case SmartType_Boolean:
            return (m_data.bool_value)? 1.0 : 0.0;
        case SmartType_Integer:
            return static_cast<double>(m_data.long_value);
        case SmartType_Double:
            return m_data.double_value;
        default:
            return invalid_double_value;
    }

    return invalid_double_value;
}

// =============================================================
// BOOL TYPE SUPPORT
// =============================================================

NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(bool InitialValue)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
    set_value_bool(InitialValue);
}

NsAppLink::NsSmartObjects::CSmartObject::operator bool(void) const
{
    return convert_bool();
}

bool NsAppLink::NsSmartObjects::CSmartObject::asBool()
{
    return (bool)(*this);
}

NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator=(bool NewValue)
{
    if(m_type != SmartType_Invalid)
    {
        set_value_bool(NewValue);
    }
    return *this;
}

bool NsAppLink::NsSmartObjects::CSmartObject::operator==(bool Value) const
{
    bool comp = convert_bool();
    if(comp == NsAppLink::NsSmartObjects::invalid_bool_value)
    {
        return false;
    }
    else
    {
        return comp == Value;
    }
}

void NsAppLink::NsSmartObjects::CSmartObject::set_value_bool(bool NewValue)
{
    set_new_type(SmartType_Boolean);
    m_data.bool_value = NewValue;
}

bool NsAppLink::NsSmartObjects::CSmartObject::convert_bool(void) const
{
    switch (m_type) {
        case SmartType_Boolean :
            return m_data.bool_value;
        case SmartType_Integer :
            return (m_data.long_value != 0);
            break;
        case SmartType_Double :
            return (m_data.double_value != 0.0);
            break;
        default :
            break;
    }

    return invalid_bool_value;
}



// =============================================================
// CHAR TYPE SUPPORT
// =============================================================


NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(char InitialValue)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
    set_value_char(InitialValue);
}

NsAppLink::NsSmartObjects::CSmartObject::operator char(void) const
{
    return convert_char();
}

char NsAppLink::NsSmartObjects::CSmartObject::asChar()
{
    return (char)(*this);
}

NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator=(char NewValue)
{
    if(m_type != SmartType_Invalid)
    {
        set_value_char(NewValue);
    }
    return *this;
}

bool NsAppLink::NsSmartObjects::CSmartObject::operator==(char Value) const
{
    char comp = convert_char();
    if(comp == NsAppLink::NsSmartObjects::invalid_char_value)
    {
        return false;
    }
    else
    {
        return comp == Value;
    }
}

void NsAppLink::NsSmartObjects::CSmartObject::set_value_char(char NewValue)
{
    set_new_type(SmartType_Character);
    m_data.char_value = NewValue;
}


char NsAppLink::NsSmartObjects::CSmartObject::convert_char(void) const
{
    switch (m_type) {
        case SmartType_String:
            return (m_data.str_value->length() == 1) ? m_data.str_value->at(0) : invalid_char_value;
        case SmartType_Character:
            return m_data.char_value;
        default :
            break;
    }

    return invalid_char_value;
}


// =============================================================
// STD::STRING TYPE SUPPORT
// =============================================================

NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(const std::string InitialValue)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
    set_value_string(InitialValue);
}

NsAppLink::NsSmartObjects::CSmartObject::operator std::string(void) const
{
    return convert_string();
}

std::string NsAppLink::NsSmartObjects::CSmartObject::asString()
{
    return (std::string)(*this);
}

NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator=(const std::string& NewValue)
{
    if(m_type != SmartType_Invalid)
    {
        set_value_string(NewValue);
    }
    return *this;
}

bool NsAppLink::NsSmartObjects::CSmartObject::operator==(std::string Value) const
{
    std::string comp = convert_string();
    if(comp == NsAppLink::NsSmartObjects::invalid_string_value)
    {
        return false;
    }
    else
    {
        return comp == Value;
    }
}

void NsAppLink::NsSmartObjects::CSmartObject::set_value_string(const std::string& NewValue)
{
    set_new_type(SmartType_String);
    m_data.str_value = new std::string(NewValue);
}

std::string NsAppLink::NsSmartObjects::CSmartObject::convert_string(void) const
{
    switch (m_type) {
        case SmartType_String :
            return *(m_data.str_value);
        case SmartType_Integer:
            return std::to_string(m_data.long_value);
            break;
        case SmartType_Character:
            return std::string(1, m_data.char_value);
            break;
        case SmartType_Double:
            return convert_double_to_string(m_data.double_value);
            break;
        default:
            break;
    }
    return invalid_string_value;
}

// =============================================================
// CHAR* TYPE SUPPORT
// =============================================================

NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(char* InitialValue)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
    set_value_cstr(InitialValue);
    return;
}
/*
NsAppLink::NsSmartObjects::CSmartObject::operator char*(void) const
{
    return convert_string().c_str();
}
*/
NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator=(const char* NewValue)
{
    if(m_type != SmartType_Invalid)
    {
        set_value_cstr(NewValue);
    }
    return *this;
}

bool NsAppLink::NsSmartObjects::CSmartObject::operator==(const char* Value) const
{
    std::string comp = convert_string();
    std::string val(Value);
    if(comp == NsAppLink::NsSmartObjects::invalid_string_value)
    {
        return false;
    }
    else
    {
        return comp == val;
    }
}

void NsAppLink::NsSmartObjects::CSmartObject::set_value_cstr(const char* NewValue)
{
    if (NewValue)
        set_value_string(std::string(NewValue));
    else {
        std::string tmp;
        set_value_string(tmp);
    }
}

// =============================================================
// BINARY TYPE SUPPORT
// =============================================================
NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(SmartBinary InitialValue)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
    set_value_binary(InitialValue);
}

NsAppLink::NsSmartObjects::CSmartObject::operator SmartBinary(void) const
{
    return convert_binary();
}

NsAppLink::NsSmartObjects::SmartBinary NsAppLink::NsSmartObjects::CSmartObject::asBinary()
{
    //return static_cast<NsAppLink::NsSmartObjects::SmartBinary>(*this);
    return convert_binary();
}

NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator=(SmartBinary NewValue)
{
    if(m_type != SmartType_Invalid)
    {
        set_value_binary(NewValue);
    }
    return *this;
}

bool NsAppLink::NsSmartObjects::CSmartObject::operator==(SmartBinary Value) const
{
    SmartBinary comp = convert_binary();
    if(comp == NsAppLink::NsSmartObjects::invalid_binary_value)
    {
        return false;
    }
    else
    {
        return std::equal(comp.begin(), comp.end(), Value.begin());
    }
}

void NsAppLink::NsSmartObjects::CSmartObject::set_value_binary(SmartBinary NewValue)
{
    set_new_type(SmartType_Binary);
    m_data.binary_value = new SmartBinary(NewValue);
}

NsAppLink::NsSmartObjects::SmartBinary NsAppLink::NsSmartObjects::CSmartObject::convert_binary(void) const
{
    switch (m_type) {
        case SmartType_Binary:
            return *(m_data.binary_value);
        default:
            return invalid_binary_value;
    }

    return invalid_binary_value;
}

// =============================================================
// ARRAY INTERFACE SUPPORT
// =============================================================

NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator[](int Index)
{
    return handle_array_access(Index);
}

inline NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::handle_array_access(int Index)
{
    if(m_type == SmartType_Invalid)
    {
        return *this;
    }

    if(m_type != SmartType_Array)
    {
        cleanup_data();
        m_type = SmartType_Array;
        m_data.array_value = new SmartArray();
    }

    int sz = m_data.array_value->size();
    if (Index == -1)
    {
        Index = sz;
    }
    if (Index == sz)
    {
        NsAppLink::NsSmartObjects::CSmartObject uc;
        m_data.array_value->push_back(uc);
    }
    if (Index > sz || Index < 0)
    {
        return invalid_object_value;
    }
    return m_data.array_value->at(Index);
}

// =============================================================
// MAP INTERFACE SUPPORT
// =============================================================

NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator[](const std::string Key)
{
    return handle_map_access(Key);
}

NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator[](char* Key)
{
    std::string str(Key);
    return handle_map_access(str);
}

NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator[](const char* Key)
{
    std::string str(Key);
    return handle_map_access(str);
}

const NsAppLink::NsSmartObjects::CSmartObject & NsAppLink::NsSmartObjects::CSmartObject::getElement(size_t Index) const
{
    if (NsAppLink::NsSmartObjects::SmartType_Array == m_type)
    {
        if (Index < m_data.array_value->size())
        {
            return m_data.array_value->at(Index);
        }
    }

    return invalid_object_value;
}

const NsAppLink::NsSmartObjects::CSmartObject & NsAppLink::NsSmartObjects::CSmartObject::getElement(const std::string & Key) const
{
    if (NsAppLink::NsSmartObjects::SmartType_Map == m_type)
    {
        SmartMap::const_iterator i = m_data.map_value->find(Key);

        if (i != m_data.map_value->end())
        {
            return i->second;
        }
    }

    return invalid_object_value;
}

NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::handle_map_access(const std::string Key)
{
    if(m_type == SmartType_Invalid)
    {
        return *this;
    }

    //TODO: implement handling of non-existing keys similar to array
    if(m_type != SmartType_Map)
    {
        cleanup_data();
        m_type = SmartType_Map;
        m_data.map_value = new NsAppLink::NsSmartObjects::SmartMap();
    }

    //TODO: Add check for key presense
    return (*(m_data.map_value))[Key];
}

// =============================================================
// OTHER METHODS
// =============================================================
void NsAppLink::NsSmartObjects::CSmartObject::duplicate(const NsAppLink::NsSmartObjects::CSmartObject& OtherObject)
{
    SmartData newData;
    SmartType newType = OtherObject.m_type;
    CSmartSchema newSchema = OtherObject.m_schema;

    switch(newType) {
        case SmartType_Map :
            newData.map_value = new SmartMap(*OtherObject.m_data.map_value);
            break;
        case SmartType_Array :
            newData.array_value = new SmartArray(*OtherObject.m_data.array_value);
            break;
        case SmartType_Integer:
            newData.long_value = OtherObject.m_data.long_value;
            break;
        case SmartType_Double :
            newData.double_value = OtherObject.m_data.double_value;
            break;
        case SmartType_Boolean :
            newData.bool_value = OtherObject.m_data.bool_value;
            break;
        case SmartType_Character :
            newData.char_value = OtherObject.m_data.char_value;
            break;
        case SmartType_String :
            newData.str_value = new std::string(*OtherObject.m_data.str_value);
            break;
        case SmartType_Binary :
            newData.binary_value = new SmartBinary(*OtherObject.m_data.binary_value);
            break;
        default:
            break;
    }

    cleanup_data();

    m_type = newType;
    m_data = newData;
    m_schema = newSchema;
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
    case SmartType_Binary :
        delete m_data.binary_value;
        break;
    default :
        break;
    }
}

size_t NsAppLink::NsSmartObjects::CSmartObject::length() const
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
    default:
        size = 0;
        break;
    }

    return size;
}

void NsAppLink::NsSmartObjects::CSmartObject::set_new_type(SmartType NewType)
{
    cleanup_data();
    m_type = NewType;
}

double NsAppLink::NsSmartObjects::CSmartObject::convert_string_to_double(const std::string* Value)
{
    if(0 == Value->size())
    {
        return invalid_double_value;
    }

    char firstSymbol = Value->at(0);

    if( (firstSymbol != '.') && (firstSymbol != '+') && (firstSymbol != '-') && ((firstSymbol < '0') || (firstSymbol > '9')) )
    {
        return invalid_double_value;
    }

    char* ptr;
    errno = 0;

    double result = strtod(Value->c_str(),&ptr);
    if (errno || (ptr != (Value->c_str() + Value->length())))
    {
        return invalid_double_value;
    }

    return result;
}


std::string NsAppLink::NsSmartObjects::CSmartObject::convert_double_to_string(const double& Value)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(10) << Value;         //convert double to string w fixed notation, hi precision
    std::string s = ss.str();                                   //output to std::string
    s.erase(s.find_last_not_of('0') + 1, std::string::npos);    //remove trailing 000s    (123.1200 => 123.12,  123.000 => 123.)
    if(s[s.size()-1] == '.') {
        s.erase(s.end()-1);                                     //remove dangling decimal (123. => 123)
    }
    return s;
}

long NsAppLink::NsSmartObjects::CSmartObject::convert_string_to_long(const std::string* Value)
{
    if(0 == Value->size())
    {
        return invalid_int_value;
    }

    char firstSymbol = Value->at(0);

    if( (firstSymbol != '+') && (firstSymbol != '-') && ((firstSymbol < '0') || (firstSymbol > '9')) )
    {
        return invalid_int_value;
    }

    char* ptr;
    errno = 0;
    long result = strtol(Value->c_str(),&ptr,10);
    if (errno || (ptr != (Value->c_str() + Value->length())))
    {
        return invalid_int_value;
    }

    return result;
}

NsAppLink::NsSmartObjects::SmartType NsAppLink::NsSmartObjects::CSmartObject::getType() const
{
    return m_type;
}

std::set<std::string> NsAppLink::NsSmartObjects::CSmartObject::enumerate() const
{
    std::set<std::string> keys;

    if(m_type == SmartType_Map)
    {
        std::transform(
            m_data.map_value->begin(),
            m_data.map_value->end(),
            std::inserter(keys, keys.end()),
            [](const SmartMap::value_type &pair){return pair.first;}
        );
    }

    return keys;
}

bool NsAppLink::NsSmartObjects::CSmartObject::keyExists(const std::string & Key) const
{
    if(m_type != SmartType_Map)
    {
        return false;
    }

    return m_data.map_value->count(Key);
}

NsAppLink::NsSmartObjects::Errors::eType NsAppLink::NsSmartObjects::CSmartObject::isValid()
{
    return m_schema.validate(*this);
}

void NsAppLink::NsSmartObjects::CSmartObject::setSchema(NsAppLink::NsSmartObjects::CSmartSchema schema)
{
    m_schema = schema;
}

NsAppLink::NsSmartObjects::CSmartSchema NsAppLink::NsSmartObjects::CSmartObject::getSchema()
{
    return m_schema;
}
