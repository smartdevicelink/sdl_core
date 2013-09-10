#ifndef SRC_COMPONENTS_DBUS_SCHEMA_INCLUDE_DBUS_SCHEMA_FORD_MESSAGE_DESCRIPTIONS_H_
#define SRC_COMPONENTS_DBUS_SCHEMA_INCLUDE_DBUS_SCHEMA_FORD_MESSAGE_DESCRIPTIONS_H_

#include <stdint.h>
#include <stdlib.h>
#include "src/components/interfaces/HMI_API.h"

namespace ford_message_descriptions
{

enum ParameterType {Integer, Boolean, Float, String, Struct, Enum};

struct ParameterDescription {
  const char* name;
  const ParameterType type;
  const bool is_array;
  const bool obligatory;
};

struct StructDescription {
  const ParameterDescription base;
  const ParameterDescription** parameters;
};

struct EnumDescription {
  const ParameterDescription base;
  struct Entry {
    const char* name;
    const uint32_t value;
  };
  const Entry** entries;
};

struct MessageDescription {
  const char* interface;
  const char* name;
  const hmi_apis::messageType::eType message_type;
  const hmi_apis::FunctionID::eType function_id;
  const ParameterDescription** parameters;
};

extern const MessageDescription* message_descriptions[];

}

#endif  // SRC_COMPONENTS_DBUS_SCHEMA_INCLUDE_DBUS_SCHEMA_FORD_MESSAGE_DESCRIPTIONS_H_

