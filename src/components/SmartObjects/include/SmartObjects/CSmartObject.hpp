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

        /**
         * @brief Enumeration with all types, supported by SmartObject
         **/
        enum SmartType
        {
            /**
             * @brief Null value. Act as initial value.
             **/
            SmartType_Null      = 0,

            /**
             * @brief Boolean value.
             **/
            SmartType_Boolean   = 1,

            /**
             * @brief Integer value.
             **/
            SmartType_Integer   = 2,

            /**
             * @brief Character value.
             **/
            SmartType_Character = 3,

            /**
             * @brief String value.
             **/
            SmartType_String    = 4,

            /**
             * @brief Double value.
             **/
            SmartType_Double    = 5,

            /**
             * @brief Map value. Gives possibility for object to act like hashtable.
             **/
            SmartType_Map       = 6,

            /**
             * @brief Array value. Gives possibility for object to act like array.
             **/
            SmartType_Array     = 7,

            /**
             * @brief Invalid value. Represents invalid object that cannot change his type.
             **/
            SmartType_Invalid   = -1
        };

        /**
         * @brief SmartArray type
         **/
        typedef std::vector<CSmartObject> SmartArray;

        /**
         * @brief SmartMap type
         **/
        typedef std::map<std::string, CSmartObject> SmartMap;

        /**
         * @brief Main SmartObject class
         *
         * This class act as Variant type from other languages and can be used as primitive type
         * like bool, int, char, double, string and as complex type like array and map.
         **/
        class CSmartObject
        {
        public:
            /**
             * @brief Constructor.
             *
             * Creates object with Null type.
             **/
            CSmartObject();

            /**
             * @brief Copy constructor.
             *
             * @param Other Object to be copied from.
             **/
            CSmartObject(const CSmartObject& Other);

            /**
             * @brief Constructor for creating object of given primitive type.
             *
             * Only primitive types (bool, int, char, double, string) are supported.
             *
             * @param type Type of the created object.
             **/
            CSmartObject(SmartType Type);

            /**
             * @brief Destructor
             *
             **/
            ~CSmartObject();

            /**
             * @brief Assignment operator.
             *
             * @param  Other Other SmartObject
             * @return CSmartObject&
             **/
            CSmartObject& operator=(const CSmartObject& Other);

            /**
             * @brief Comparison operator
             *
             * @param  Other Other SmartObject to be compared with
             * @return bool Result of comparison
             **/
            bool operator==(const CSmartObject& Other) const;

            /**
             * @name Support of type: int
             * @{
             */
            /**
             * @brief Constructor for creating object of type: int
             *
             * @param InitialValue Initial object value
             **/
            CSmartObject(int InitialValue);

            /**
             * @brief Conversion operator to type: int
             *
             * @return int Value of the object converted to int type or invalid_int_value if
             *         conversion is not possible
             **/
            operator int(void) const;

            /**
             * @brief Assignment operator for type: int
             *
             * @param  NewValue New object value
             * @return CSmartObject&
             **/
            CSmartObject& operator=(int NewValue);

            /**
             * @brief Comparison operator for comparing object with integer value
             *
             * @param  Value Value to compare object with
             * @return bool
             **/
            bool operator==(int Value) const;

            // Support of type: long
            /**
             * @brief Constructor for creating object of type: int
             *
             * @param InitialValue Initial object value
             **/
            CSmartObject(long InitialValue);

            /**
             * @brief Conversion operator to type: int
             *
             * @return long Value of the object converted to int type or invalid_int_value if
             *         conversion is not possible
             **/
            operator long(void) const;

            /**
             * @brief Assignment operator for type: int
             *
             * @param  NewValue New object value
             * @return CSmartObject&
             **/
            CSmartObject& operator=(long NewValue);

            /**
             * @brief Comparison operator for comparing object with long value
             *
             * @param  Value Value to compare object with
             * @return bool
             **/
            bool operator==(long Value) const;

            /** @} */

            /**
             * @name Support of type: double
             * @{
             */
            /**
             * @brief Constructor for creating object of type: double
             *
             * @param InitialValue Initial object value
             **/
            CSmartObject(double InitialValue);

            /**
             * @brief Conversion operator to type: double
             *
             * @return double Value of the object converted to double type or invalid_double_value if
             *         conversion is not possible
             **/
            operator double(void) const;

            /**
             * @brief Assignment operator for type: double
             *
             * @param  NewValue New object value
             * @return CSmartObject&
             **/
            CSmartObject& operator=(double);

            /**
             * @brief Comparison operator for comparing object with double value
             *
             * @param  Value Value to compare object with
             * @return bool
             **/
            bool operator==(double) const;
            /** @} */

            /**
             * @name Support of type: bool
             * @{
             */
            /**
             * @brief Constructor for creating object of type: bool
             *
             * @param InitialValue Initial object value
             **/
            CSmartObject(bool InitialValue);

            /**
             * @brief Conversion operator to type: bool
             *
             * @return bool Value of the object converted to bool type or invalid_bool_value if
             *         conversion is not possible
             **/
            operator bool(void) const;

            /**
             * @brief Assignment operator for type: bool
             *
             * @param  NewValue New object value
             * @return CSmartObject&
             **/
            CSmartObject& operator=(bool NewValue);

            /**
             * @brief Comparison operator for comparing object with bool value
             *
             * @param  Value Value to compare object with
             * @return bool
             **/
            bool operator==(bool Value) const;
            /** @} */

            /**
             * @name Support of type: char
             * @{
             */
            /**
             * @brief Constructor for creating object of type: char
             *
             * @param InitialValue Initial object value
             **/
            CSmartObject(char InitialValue);

            /**
             * @brief Conversion operator to type: char
             *
             * @return char Value of the object converted to bool type or invalid_char_value if
             *         conversion is not possible
             **/
            operator char(void) const;

            /**
             * @brief Assignment operator for type: char
             *
             * @param  NewValue New object value
             * @return CSmartObject&
             **/
            CSmartObject& operator=(char NewValue);

            /**
             * @brief Comparison operator for comparing object with char value
             *
             * @param  Value Value to compare object with
             * @return bool
             **/
            bool operator==(char Value) const;
            /** @} */

            /**
             * @name Support of type: string
             * @{
             */
            /**
             * @brief Constructor for creating object of type: string
             *
             * @param InitialValue Initial object value
             **/
            CSmartObject(const std::string InitialValue);

            /**
             * @brief Constructor for creating object of type: string
             *
             * @param InitialValue Initial object value
             **/
            CSmartObject(char* InitialValue);

            /**
             * @brief Conversion operator to type: string
             *
             * @return std::string Value of the object converted to bool type or invalid_string_value if
             *         conversion is not possible
             **/
            operator std::string(void) const;

            /**
             * @brief Assignment operator for type: string
             *
             * @param  NewValue New object value
             * @return CSmartObject&
             **/
            CSmartObject& operator=(const std::string& NewValue);

            /**
             * @brief Assignment operator for type: string
             *
             * @param  NewValue New object value
             * @return CSmartObject&
             **/
            CSmartObject& operator=(const char* NewValue);

            /**
             * @brief Comparison operator for comparing object with string value
             *
             * @param  Value Value to compare object with
             * @return bool
             **/
            bool operator==(std::string Value) const;

            /**
             * @brief Comparison operator for comparing object with string value
             *
             * @param  Value Value to compare object with
             * @return bool
             **/
            bool operator==(const char* Value) const;
            /** @} */

            /**
             * @name Array interface support
             * @{
             */
            /**
             * @brief Support of array-like access
             *
             * @param  Index index of element to return
             * @return CSmartObject&
             **/
            CSmartObject& operator[](int Index);

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

            /** @} */

            /**
             * @name Map/Hashtable interface support
             * @{
             */
            /**
             * @brief Support of map-like access
             *
             * @param  Key Key of element to return
             * @return CSmartObject&
             **/
            CSmartObject& operator[](const std::string Key);

            /**
             * @brief Support of map-like access
             *
             * @param  Key Key of element to return
             * @return CSmartObject&
             **/
            CSmartObject& operator[](char* Key);

            /**
             * @brief Support of map-like access
             *
             * @param  Key Key of element to return
             * @return CSmartObject&
             **/
            CSmartObject& operator[](const char* Key);

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

            /**
             * @brief Enumerates content of the object when it behaves like a map.
             *
             * @return Set of map keys or empty set if object has type other than map
             **/
            std::set<std::string> enumerate() const;

            /**
             * @brief Checks for key presense when object is behaves like a map
             *
             * @param Key Key to check presense for
             * @return bool
             **/
            bool keyExists(const std::string & Key) const;
            /** @} */

            /**
             * @name Validation and schema support
             * @{
             */
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
             * @return CSmartSchema
             **/
            CSmartSchema getSchema();
            /** @} */

            /**
             * @brief Returns current object type
             *
             * @return NsSmartObjects::SmartType
             **/
            SmartType getType() const;


            /**
             * @brief Returns length of object
             *
             * If object has type string, array or map then method returns corresponded
             * size. Otherwise returns zero
             *
             * @return size_t Length of the object
             **/
            size_t length() const;

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
        static CSmartObject      invalid_object_value(SmartType_Invalid);

    }
}
#endif // __CSMARTOBJECT_HPP__

