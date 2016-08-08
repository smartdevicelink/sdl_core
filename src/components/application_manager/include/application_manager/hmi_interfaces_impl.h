#ifndef HMI_INTERFACES_IMPL_H
#define HMI_INTERFACES_IMPL_H
#include <map>
#include "application_manager/hmi_interfaces.h"
#include "utils/macro.h"

namespace application_manager {
class HmiInterfacesImpl : public HmiInterfaces {
 public:
  HmiInterfacesImpl();
  ~HmiInterfacesImpl() {}
  InterfaceState GetInterfaceState(InterfaceID interface) const OVERRIDE;
  InterfaceID GetInterfaceFromFunction(
      hmi_apis::FunctionID::eType function) const OVERRIDE;
  void SetInterfaceState(InterfaceID interface, InterfaceState state) OVERRIDE;

 private:
  typedef std::map<InterfaceID, InterfaceState> InterfaceStatesMap;
  InterfaceStatesMap interfaces_states_;
};
}
#endif  // HMI_INTERFACES_IMPL_H
