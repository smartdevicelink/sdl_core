/**
 * @file generated_MOBILE_API_policy_types.h
 * @brief Generated class generated_MOBILE_API_policy_types source file.
 *
*/
#include "policy/policy_table/generated_MOBILE_API_policy_types.h"

namespace rpc {
namespace policy_table_interface_base {

bool IsValidEnum(AppHMIType val) {
  switch (val) {
    case DEFAULT:
      return true;
    case COMMUNICATION:
      return true;
    case MEDIA:
      return true;
    case MESSAGING:
      return true;
    case NAVIGATION:
      return true;
    case INFORMATION:
      return true;
    case SOCIAL:
      return true;
    case BACKGROUND_PROCESS:
      return true;
    case TESTING:
      return true;
    case SYSTEM:
      return true;
    case PROJECTION:
      return true;
    case REMOTE_CONTROL:
      return true;
    default:
      return false;
  }
};

const char* EnumToJsonString(AppHMIType val) {
  switch (val) {
    case DEFAULT:
      return "DEFAULT";
    case COMMUNICATION:
      return "COMMUNICATION";
    case MEDIA:
      return "MEDIA";
    case MESSAGING:
      return "MESSAGING";
    case NAVIGATION:
      return "NAVIGATION";
    case INFORMATION:
      return "INFORMATION";
    case SOCIAL:
      return "SOCIAL";
    case BACKGROUND_PROCESS:
      return "BACKGROUND_PROCESS";
    case TESTING:
      return "TESTING";
    case SYSTEM:
      return "SYSTEM";
    case PROJECTION:
      return "PROJECTION";
    case REMOTE_CONTROL:
      return "REMOTE_CONTROL";
    default:
      return "";
  }
};

bool EnumFromJsonString(const std::string& literal, AppHMIType* result) {
  if ("DEFAULT" == literal) {
    *result = DEFAULT;
    return true;
  }

  if ("COMMUNICATION" == literal) {
    *result = COMMUNICATION;
    return true;
  }

  if ("MEDIA" == literal) {
    *result = MEDIA;
    return true;
  }

  if ("MESSAGING" == literal) {
    *result = MESSAGING;
    return true;
  }

  if ("NAVIGATION" == literal) {
    *result = NAVIGATION;
    return true;
  }

  if ("INFORMATION" == literal) {
    *result = INFORMATION;
    return true;
  }

  if ("SOCIAL" == literal) {
    *result = SOCIAL;
    return true;
  }

  if ("BACKGROUND_PROCESS" == literal) {
    *result = BACKGROUND_PROCESS;
    return true;
  }

  if ("TESTING" == literal) {
    *result = TESTING;
    return true;
  }

  if ("SYSTEM" == literal) {
    *result = SYSTEM;
    return true;
  }

  if ("PROJECTION" == literal) {
    *result = PROJECTION;
    return true;
  }

  if ("REMOTE_CONTROL" == literal) {
    *result = REMOTE_CONTROL;
    return true;
  }
  return false;
};

}  // rpc
}  // policy_table_interface_base
