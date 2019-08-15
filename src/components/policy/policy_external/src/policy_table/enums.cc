#include "policy/policy_table/enums.h"
#include <cstring>

namespace rpc {
namespace policy_table_interface_base {

bool IsValidEnum(Input val) {
  switch (val) {
    case I_GUI:
      return true;
    case I_VUI:
      return true;
    default:
      return false;
  }
}

const char* EnumToJsonString(Input val) {
  switch (val) {
    case I_GUI:
      return "GUI";
    case I_VUI:
      return "VUI";
    default:
      return "";
  }
}
bool EnumFromJsonString(const std::string& literal, Input* result) {
  if ("GUI" == literal) {
    *result = I_GUI;
    return true;
  } else if ("VUI" == literal) {
    *result = I_VUI;
    return true;
  } else {
    return false;
  }
}

const std::string kDefaultApp = "default";
const std::string kPreDataConsentApp = "pre_DataConsent";
const std::string kDeviceApp = "device";

}  // namespace policy_table_interface_base
}  // namespace rpc
