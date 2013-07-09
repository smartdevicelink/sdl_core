/**
 * @file CSmartObject.cpp
 * @brief CSmartObject source file.
 */
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 'A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include "SmartObjects/CSmartObject.hpp"
#include <limits>
#include <errno.h>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iterator>

NsSmartDeviceLink::NsSmartObjects::CSmartObject::CSmartObject(void)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
}

NsSmartDeviceLink::NsSmartObjects::CSmartObject::CSmartObject(const NsSmartDeviceLink::NsSmartObjects::CSmartObject& Other)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
    duplicate(Other);
}

NsSmartDeviceLink::NsSmartObjects::CSmartObject::CSmartObject(SmartType Type)
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
        case SmartType_Map :
          m_data.map_value = new SmartMap();
          m_type = SmartType_Map;
          break;
        case SmartType_Array:
          m_data.array_value = new SmartArray();
          m_type = SmartType_Array;
          break;
        case SmartType_Binary :
            set_value_binary(SmartBinary());
            break;
        case SmartType_Invalid:
            m_type = SmartType_Invalid;
            break;
    }
}

NsSmartDeviceLink::NsSmartObjects::CSmartObject::~CSmartObject()
{
    cleanup_data();
}

NsSmartDeviceLink::NsSmartObjects::CSmartObject& NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator=(const NsSmartDeviceLink::NsSmartObjects::CSmartObject& Other)
{
    if (this != &Other) duplicate(Other);
    return *this;
}

bool NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator==(const NsSmartDeviceLink::NsSmartObjects::CSmartObject& Other) const
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
NsSmartDeviceLink::NsSmartObjects::CSmartObject::CSmartObject(int InitialValue)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
    set_value_integer(InitialValue);
}

NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator int(void) const
{
    return convert_int();
}

int NsSmartDeviceLink::NsSmartObjects::CSmartObject::asInt() const
{
    return (int)(*this);
}

NsSmartDeviceLink::NsSmartObjects::CSmartObject& NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator=(int NewValue)
{
    if(m_type != SmartType_Invalid)
    {
        set_value_integer(NewValue);
    }
    return *this;
}

bool NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator==(int Value) const
{
    int comp = convert_int();
    if(comp == NsSmartDeviceLink::NsSmartObjects::invalid_int_value)
    {
        return false;
    }
    else
    {
        return comp == Value;
    }
}

void NsSmartDeviceLink::NsSmartObjects::CSmartObject::set_value_integer(int NewValue)
{
    set_new_type(SmartType_Integer);
    m_data.long_value = NewValue;
}

int NsSmartDeviceLink::NsSmartObjects::CSmartObject::convert_int(void) const
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
NsSmartDeviceLink::NsSmartObjects::CSmartObject::CSmartObject(long InitialValue)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
    set_value_long(InitialValue);
}

NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator long(void) const
{
    return convert_long();
}

long NsSmartDeviceLink::NsSmartObjects::CSmartObject::asLong() const
{
    return (long)(*this);
}

NsSmartDeviceLink::NsSmartObjects::CSmartObject& NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator=(long NewValue)
{
    if(m_type != SmartType_Invalid)
    {
        set_value_integer(NewValue);
    }
    return *this;
}

bool NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator==(long Value) const
{
    int comp = convert_long();
    if(comp == NsSmartDeviceLink::NsSmartObjects::invalid_int_value)
    {
        return false;
    }
    else
    {
        return comp == Value;
    }
}

void NsSmartDeviceLink::NsSmartObjects::CSmartObject::set_value_long(long NewValue)
{
    set_new_type(SmartType_Integer);
    m_data.long_value = NewValue;
}

long NsSmartDeviceLink::NsSmartObjects::CSmartObject::convert_long(void) const
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
NsSmartDeviceLink::NsSmartObjects::CSmartObject::CSmartObject(double InitialValue)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
    set_value_double(InitialValue);
}

NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator double(void) const
{
    return convert_double();
}

double NsSmartDeviceLink::NsSmartObjects::CSmartObject::asDouble() const
{
    return (double)(*this);
}

NsSmartDeviceLink::NsSmartObjects::CSmartObject& NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator=(double NewValue)
{
    if(m_type != SmartType_Invalid)
    {
        set_value_double(NewValue);
    }
    return *this;
}

bool NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator==(double Value) const
{
    double comp = convert_double();
    if(comp == NsSmartDeviceLink::NsSmartObjects::invalid_double_value)
    {
        return false;
    }
    else
    {
        return comp == Value;
    }
}

void NsSmartDeviceLink::NsSmartObjects::CSmartObject::set_value_double(double NewValue)
{
    set_new_type(SmartType_Double);
    m_data.double_value = NewValue;
}

double NsSmartDeviceLink::NsSmartObjects::CSmartObject::convert_double(void) const
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

NsSmartDeviceLink::NsSmartObjects::CSmartObject::CSmartObject(bool InitialValue)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
    set_value_bool(InitialValue);
}

NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator bool(void) const
{
    return convert_bool();
}

bool NsSmartDeviceLink::NsSmartObjects::CSmartObject::asBool() const
{
    return (bool)(*this);
}

NsSmartDeviceLink::NsSmartObjects::CSmartObject& NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator=(bool NewValue)
{
    if(m_type != SmartType_Invalid)
    {
        set_value_bool(NewValue);
    }
    return *this;
}

bool NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator==(bool Value) const
{
    bool comp = convert_bool();
    if(comp == NsSmartDeviceLink::NsSmartObjects::invalid_bool_value)
    {
        return false;
    }
    else
    {
        return comp == Value;
    }
}

void NsSmartDeviceLink::NsSmartObjects::CSmartObject::set_value_bool(bool NewValue)
{
    set_new_type(SmartType_Boolean);
    m_data.bool_value = NewValue;
}

bool NsSmartDeviceLink::NsSmartObjects::CSmartObject::convert_bool(void) const
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


NsSmartDeviceLink::NsSmartObjects::CSmartObject::CSmartObject(char InitialValue)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
    set_value_char(InitialValue);
}

NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator char(void) const
{
    return convert_char();
}

char NsSmartDeviceLink::NsSmartObjects::CSmartObject::asChar() const
{
    return (char)(*this);
}

NsSmartDeviceLink::NsSmartObjects::CSmartObject& NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator=(char NewValue)
{
    if(m_type != SmartType_Invalid)
    {
        set_value_char(NewValue);
    }
    return *this;
}

bool NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator==(char Value) const
{
    char comp = convert_char();
    if(comp == NsSmartDeviceLink::NsSmartObjects::invalid_char_value)
    {
        return false;
    }
    else
    {
        return comp == Value;
    }
}

void NsSmartDeviceLink::NsSmartObjects::CSmartObject::set_value_char(char NewValue)
{
    set_new_type(SmartType_Character);
    m_data.char_value = NewValue;
}


char NsSmartDeviceLink::NsSmartObjects::CSmartObject::convert_char(void) const
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

NsSmartDeviceLink::NsSmartObjects::CSmartObject::CSmartObject(const std::string InitialValue)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
    set_value_string(InitialValue);
}

NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator std::string(void) const
{
    return convert_string();
}

std::string NsSmartDeviceLink::NsSmartObjects::CSmartObject::asString() const
{
    return (std::string)(*this);
}

NsSmartDeviceLink::NsSmartObjects::CSmartObject& NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator=(const std::string& NewValue)
{
    if(m_type != SmartType_Invalid)
    {
        set_value_string(NewValue);
    }
    return *this;
}

bool NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator==(std::string Value) const
{
    std::string comp = convert_string();
    if(comp == NsSmartDeviceLink::NsSmartObjects::invalid_string_value)
    {
        return false;
    }
    else
    {
        return comp == Value;
    }
}

void NsSmartDeviceLink::NsSmartObjects::CSmartObject::set_value_string(const std::string& NewValue)
{
    set_new_type(SmartType_String);
    m_data.str_value = new std::string(NewValue);
}

std::string NsSmartDeviceLink::NsSmartObjects::CSmartObject::convert_string(void) const
{
    switch (m_type) {
        case SmartType_String :
            return *(m_data.str_value);
        case SmartType_Integer:
            char val[20];
            return itoa(m_data.long_value, val, 10);
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

NsSmartDeviceLink::NsSmartObjects::CSmartObject::CSmartObject(char* InitialValue)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
    set_value_cstr(InitialValue);
    return;
}
/*
NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator char*(void) const
{
    return convert_string().c_str();
}
*/
NsSmartDeviceLink::NsSmartObjects::CSmartObject& NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator=(const char* NewValue)
{
    if(m_type != SmartType_Invalid)
    {
        set_value_cstr(NewValue);
    }
    return *this;
}

bool NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator==(const char* Value) const
{
    std::string comp = convert_string();
    std::string val(Value);
    if(comp == NsSmartDeviceLink::NsSmartObjects::invalid_string_value)
    {
        return false;
    }
    else
    {
        return comp == val;
    }
}

void NsSmartDeviceLink::NsSmartObjects::CSmartObject::set_value_cstr(const char* NewValue)
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
NsSmartDeviceLink::NsSmartObjects::CSmartObject::CSmartObject(SmartBinary InitialValue)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
    set_value_binary(InitialValue);
}

NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator SmartBinary(void) const
{
    return convert_binary();
}

NsSmartDeviceLink::NsSmartObjects::SmartBinary NsSmartDeviceLink::NsSmartObjects::CSmartObject::asBinary() const
{
    //return static_cast<NsSmartDeviceLink::NsSmartObjects::SmartBinary>(*this);
    return convert_binary();
}

NsSmartDeviceLink::NsSmartObjects::CSmartObject& NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator=(SmartBinary NewValue)
{
    if(m_type != SmartType_Invalid)
    {
        set_value_binary(NewValue);
    }
    return *this;
}

bool NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator==(SmartBinary Value) const
{
    SmartBinary comp = convert_binary();
    if(comp == NsSmartDeviceLink::NsSmartObjects::invalid_binary_value)
    {
        return false;
    }
    else
    {
        return std::equal(comp.begin(), comp.end(), Value.begin());
    }
}

void NsSmartDeviceLink::NsSmartObjects::CSmartObject::set_value_binary(SmartBinary NewValue)
{
    set_new_type(SmartType_Binary);
    m_data.binary_value = new SmartBinary(NewValue);
}

NsSmartDeviceLink::NsSmartObjects::SmartBinary NsSmartDeviceLink::NsSmartObjects::CSmartObject::convert_binary(void) const
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

NsSmartDeviceLink::NsSmartObjects::CSmartObject& NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator[](int Index)
{
    return handle_array_access(Index);
}

inline NsSmartDeviceLink::NsSmartObjects::CSmartObject& NsSmartDeviceLink::NsSmartObjects::CSmartObject::handle_array_access(int Index)
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
        NsSmartDeviceLink::NsSmartObjects::CSmartObject uc;
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

NsSmartDeviceLink::NsSmartObjects::CSmartObject& NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator[](const std::string Key)
{
    return handle_map_access(Key);
}

NsSmartDeviceLink::NsSmartObjects::CSmartObject& NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator[](char* Key)
{
    std::string str(Key);
    return handle_map_access(str);
}

NsSmartDeviceLink::NsSmartObjects::CSmartObject& NsSmartDeviceLink::NsSmartObjects::CSmartObject::operator[](const char* Key)
{
    std::string str(Key);
    return handle_map_access(str);
}

const NsSmartDeviceLink::NsSmartObjects::CSmartObject & NsSmartDeviceLink::NsSmartObjects::CSmartObject::getElement(size_t Index) const
{
    if (NsSmartDeviceLink::NsSmartObjects::SmartType_Array == m_type)
    {
        if (Index < m_data.array_value->size())
        {
            return m_data.array_value->at(Index);
        }
    }

    return invalid_object_value;
}

const NsSmartDeviceLink::NsSmartObjects::CSmartObject & NsSmartDeviceLink::NsSmartObjects::CSmartObject::getElement(const std::string & Key) const
{
    if (NsSmartDeviceLink::NsSmartObjects::SmartType_Map == m_type)
    {
        SmartMap::const_iterator i = m_data.map_value->find(Key);

        if (i != m_data.map_value->end())
        {
            return i->second;
        }
    }

    return invalid_object_value;
}

NsSmartDeviceLink::NsSmartObjects::CSmartObject& NsSmartDeviceLink::NsSmartObjects::CSmartObject::handle_map_access(const std::string Key)
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
        m_data.map_value = new NsSmartDeviceLink::NsSmartObjects::SmartMap();
    }

    //TODO: Add check for key presense
    return (*(m_data.map_value))[Key];
}

// =============================================================
// OTHER METHODS
// =============================================================
void NsSmartDeviceLink::NsSmartObjects::CSmartObject::duplicate(const NsSmartDeviceLink::NsSmartObjects::CSmartObject& OtherObject)
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

void NsSmartDeviceLink::NsSmartObjects::CSmartObject::cleanup_data()
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

size_t NsSmartDeviceLink::NsSmartObjects::CSmartObject::length() const
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

void NsSmartDeviceLink::NsSmartObjects::CSmartObject::set_new_type(SmartType NewType)
{
    cleanup_data();
    m_type = NewType;
}

double NsSmartDeviceLink::NsSmartObjects::CSmartObject::convert_string_to_double(const std::string* Value)
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


std::string NsSmartDeviceLink::NsSmartObjects::CSmartObject::convert_double_to_string(const double& Value)
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

long NsSmartDeviceLink::NsSmartObjects::CSmartObject::convert_string_to_long(const std::string* Value)
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

NsSmartDeviceLink::NsSmartObjects::SmartType NsSmartDeviceLink::NsSmartObjects::CSmartObject::getType() const
{
    return m_type;
}

std::string NsSmartDeviceLink::NsSmartObjects::CSmartObject::OperatorToTransform(const SmartMap::value_type &pair) {
    return pair.first;
}

std::set<std::string> NsSmartDeviceLink::NsSmartObjects::CSmartObject::enumerate() const
{
    std::set<std::string> keys;

    if(m_type == SmartType_Map)
    {
        std::transform(
            m_data.map_value->begin(),
            m_data.map_value->end(),
            std::inserter(keys, keys.end()),
            //operator[](const SmartMap::value_type &pair){return pair.first;}
            &NsSmartDeviceLink::NsSmartObjects::CSmartObject::OperatorToTransform
        );
    }

    return keys;
}

bool NsSmartDeviceLink::NsSmartObjects::CSmartObject::keyExists(const std::string & Key) const
{
    if(m_type != SmartType_Map)
    {
        return false;
    }

    return m_data.map_value->count(Key);
}

bool NsSmartDeviceLink::NsSmartObjects::CSmartObject::erase(const std::string & Key)
{
    if (m_type != SmartType_Map)
    {
        return false;
    }

    return (1 == m_data.map_value->erase(Key));
}

NsSmartDeviceLink::NsSmartObjects::Errors::eType NsSmartDeviceLink::NsSmartObjects::CSmartObject::isValid()
{
    return m_schema.validate(*this);
}

void NsSmartDeviceLink::NsSmartObjects::CSmartObject::setSchema(NsSmartDeviceLink::NsSmartObjects::CSmartSchema schema)
{
    m_schema = schema;
}

NsSmartDeviceLink::NsSmartObjects::CSmartSchema NsSmartDeviceLink::NsSmartObjects::CSmartObject::getSchema()
{
    return m_schema;
}
