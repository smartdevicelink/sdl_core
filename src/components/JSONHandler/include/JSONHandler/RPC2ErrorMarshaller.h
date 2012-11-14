/**
* \file RPC2ErrorMarshaller.hpp
* \brief RPC2ErrorMarshaller class header.
* \author PVyshnevska
*/


#ifndef RPC2ERRORMARSHALLER_INCLUDE
#define RPC2ERRORMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>
#include "RPC2Error.h"

/**
  *\namespace RPC2Communication
  *\brief Namespace for communication with HMI.
*/
namespace RPC2Communication
{
    /**
     * \class RPC2ErrorMarshaller
     * \brief For serializing/deserializing of error messages.
    */
  struct RPC2ErrorMarshaller
  {
    /**
     * \brief Deserialized object form string.
     * \param s Json string
     * \param e Output: deserialized object.
     * \return Whether deserialization was successful.
     */
    static bool fromString(const std::string& s,RPC2Error& e);

    /**
     * \brief Serializes object to string.
     * \param e Error object to be serialized.
     * \return Json string.
     */
    static const std::string toString(const RPC2Error& e);

    /**
     * \brief Deserialized object from Json
     * \param s Json string
     * \param e Output: deserialized object.
     * \return Whether deserialization was successful.
     */
    static bool fromJSON(const Json::Value& s,RPC2Error& e);

    /**
     * \brief Serializes object to Json.
     * \param e Error object to be serialized.
     * \return Json value.
     */
    static Json::Value toJSON(const RPC2Error& e);
  };
}


#endif //  RPC2ERRORMARSHALLER_INCLUDE
