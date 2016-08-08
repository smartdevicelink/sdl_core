#ifndef HMI_INTERFACES_H
#define HMI_INTERFACES_H
#include "interfaces/HMI_API.h"

namespace application_manager {

class HmiInterfaces {
 public:
  enum InterfaceID {
    HMI_INTERFACE_INVALID_ENUM,
    HMI_INTERFACE_Buttons,
    HMI_INTERFACE_BasicCommunication,
    HMI_INTERFACE_VR,
    HMI_INTERFACE_TTS,
    HMI_INTERFACE_UI,
    HMI_INTERFACE_Navigation,
    HMI_INTERFACE_VehicleInfo,
    HMI_INTERFACE_SDL
  };

  enum InterfaceState {
    STATE_NOT_RESPONSE,
    STATE_AVAILABLE,
    STATE_NOT_AVAILABLE
  };

  virtual InterfaceState GetInterfaceState(InterfaceID interface) const = 0;
  virtual InterfaceID GetInterfaceFromFunction(
      hmi_apis::FunctionID::eType function) const = 0;
  virtual void SetInterfaceState(InterfaceID interface,
                                InterfaceState state) = 0;
  virtual ~HmiInterfaces() {}
};
}
#endif  // HMI_INTERFACES_H
