#include "can_cooperation/can_app_extension.h"

namespace can_cooperation {
CANAppExtenstion::CANAppExtenstion(application_manager::AppExtensionUID uid)
  : AppExtension(uid) ,
    is_control_given_(false) {
}

bool CANAppExtenstion::IsControlGiven() const {
  return is_control_given_;
}

void CANAppExtenstion::GiveControl(bool is_control_given) {
  is_control_given_ = is_control_given;
}

CANAppExtenstion::~CANAppExtenstion() {}
}  //  namespace can_cooperation
