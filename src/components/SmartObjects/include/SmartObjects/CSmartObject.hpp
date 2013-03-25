#ifndef __CSMARTOBJECT_HPP__
#define __CSMARTOBJECT_HPP__

#include <set>
#include <string>
#include <vector>
#include <map>

#include "CSmartSchema.hpp"

namespace NsAppLink
{
    namespace NsSmartObjects
    {
        class CSmartObject;

        enum SmartType
        {
            SmartType_Null      = 0,
            SmartType_Boolean   = 1,
            SmartType_Integer   = 2,
            SmartType_Character = 3,
            SmartType_String    = 4,
            SmartType_Double    = 5,
            SmartType_Map       = 6,
            SmartType_Array     = 7,
            SmartType_Invalid   = -1
        };

        typedef std::vector<CSmartObject> SmartArray;
        typedef std::map<std::string, CSmartObject> SmartMap;

        class CSmartObject
        {
        public:
            CSmartObject();
            CSmartObject(const CSmartObject&);
            CSmartObject(SmartType type);
            ~CSmartObject();

            CSmartObject& operator=(const CSmartObject&);
            bool operator==(const CSmartObject&) const;

            // Support of type: int
            CSmartObject(int);
            operator int(void) const;
            CSmartObject& operator=(int);
            bool operator==(int) const;

            // Support of type: long
            CSmartObject(long);
            operator long(void) const;
            CSmartObject& operator=(long);
            bool operator==(long) const;

            // Support of type: double
            CSmartObject(double);
            operator double(void) const;
            CSmartObject& operator=(double);
            bool operator==(double) const;

            // Support of type: bool
            CSmartObject(bool);
            operator bool(void) const;
            CSmartObject& operator=(bool);
            bool operator==(bool) const;

            // Support of type: char
            CSmartObject(char);
            operator char(void) const;
            CSmartObject& operator=(char);
            bool operator==(char) const;

            // Support of type: std::string
            CSmartObject(const std::string);
            operator std::string(void) const;
            CSmartObject& operator=(const std::string&);
            bool operator==(std::string) const;

            // Support of type: char*
            CSmartObject(char*);
            //operator char*(void) const;
            CSmartObject& operator=(const char*);
            bool operator==(const char*) const;

            // Array interface support
            CSmartObject& operator[](int);

            // Map interface support
            CSmartObject& operator[](const std::string s);
            CSmartObject& operator[](char* s);
            CSmartObject& operator[](const char*);

            /**
             * @brief Get array element.
             *
             * This method does not automatically convert the object to an array or
             * add elements. If this object is not an array or index is out of
             * range then null object is returned.
             *
             * @param Index Index of an element.
             *
             * @return Element of array or null object if element can't be provided.
             **/
            const CSmartObject & getElement(size_t Index) const;

            /**
             * @brief Get map element.
             *
             * This method does not automatically convert the object to a map or
             * add elements. If this object is not a map or it does not contain
             * key then null object is returned.
             *
             * @param Key Key of an element.
             *
             * @return Element of map or null object if element can't be provided.
             **/
            const CSmartObject & getElement(const std::string & Key) const;

            SmartType get_type() const;


            size_t length() const;


            /**
             * @brief Enumerates content of the object when it behaves like a map.
             *
             * Returns set of map keys
             **/
            std::set<std::string> enumerate() const;

            bool keyExists(const std::string & Key) const;

            //TODO: Implement method keyExist(const char*) for checking key in map availability

            /**
             * @brief Validates object according to attached schema
             *
             * @return bool
             **/
            Errors::eType isValid();

            /**
             * @brief Sets new schema
             *
             * @param schema Schema for object validation
             * @return void
             **/
            void setSchema(CSmartSchema schema);

            /**
             * @brief Returns attached schema
             *
             * @return :NsSmartObjects::CSmartSchema
             **/
            CSmartSchema getSchema();

        protected:
            SmartType m_type;

            typedef union {
                double double_value;
                bool bool_value;
                char char_value;
                long long_value;
                std::string* str_value;
                SmartArray* array_value;
                SmartMap* map_value;
            } SmartData;

            SmartData m_data;
            CSmartSchema m_schema;

            // Support of type: int
            inline void set_value_integer(int);
            inline int convert_int(void) const;

            // Support of type: char
            inline void set_value_char(char);
            inline char convert_char(void) const;

            // Support of type: double
            inline void set_value_double(double);
            inline double convert_double(void) const;

            // Support of type: bool
            inline void set_value_bool(bool);
            inline bool convert_bool(void) const;

            // Support of type: string
            inline void set_value_string(const std::string&);
            inline void set_value_cstr(const char*);
            inline std::string convert_string(void) const;

            // Support of type: long
            inline void set_value_long(long);
            inline long convert_long(void) const;

            // Array interface support
            inline CSmartObject& handle_map_access(std::string);

            // Map interface support
            CSmartObject& handle_array_access(int index);

            // Helper methods
            static double convert_string_to_double(const std::string* s);
            static long convert_string_to_long(const std::string* s);
            static std::string convert_double_to_string(const double& value);

            void duplicate(const CSmartObject&);
            void cleanup_data();
            // TODO: Is it really necessary to be that long? Why not just "set_new_type"
            void cleanup_data_if_type_changed_and_set_new_type(SmartType newType);
        };

        static const bool        invalid_bool_value   = false;
        static const int         invalid_int_value    = -1;
        static const char        invalid_char_value   = 0;
        static const std::string invalid_string_value = "";
        static const double      invalid_double_value   = -1;
        static const char*       invalid_cstr_value   = "";
        static CSmartObject      invalid_object_value;

    }
}
#endif // __CSMARTOBJECT_HPP__

