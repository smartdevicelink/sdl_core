#ifndef HMI_INTERFACES_IMPL_H
#define HMI_INTERFACES_IMPL_H
#include <map>
#include "application_manager/hmi_interfaces.h"
#include "utils/macro.h"

namespace application_manager {
class HmiInterfacesImpl : public HmiInterfaces {
 public:
  HmiInterfacesImpl() {}
  ~HmiInterfacesImpl() {}
  InterfaceState GetInterfaceState(InterfaceName interface) const OVERRIDE;
  InterfaceName GetInterfaceFromFunction(
      hmi_apis::FunctionID::eType function) const OVERRIDE;
  void SetInterfaceState(InterfaceName interface, InterfaceState state) OVERRIDE;

 private:
  typedef std::map<InterfaceName, InterfaceState> InterfaceStatesMap;
  InterfaceStatesMap interfaces_states_;
};
}
#endif  // HMI_INTERFACES_IMPL_H
