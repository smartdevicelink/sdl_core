#include "can_cooperation/can_app_extension.h"

namespace can_cooperation {
CANAppExtension::CANAppExtension(application_manager::AppExtensionUID uid)
  : AppExtension(uid) ,
    is_control_given_(false) {
}

bool CANAppExtension::IsControlGiven() const {
  return is_control_given_;
}

void CANAppExtension::GiveControl(bool is_control_given) {
  is_control_given_ = is_control_given;
}

CANAppExtension::~CANAppExtension() {}
}  //  namespace can_cooperation
