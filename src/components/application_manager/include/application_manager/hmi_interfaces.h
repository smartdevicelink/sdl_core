#ifndef HMI_INTERFACES_H
#define HMI_INTERFACES_H
#include "interfaces/HMI_API.h"

namespace application_manager {

class HmiInterfaces {
 public:
  enum InterfaceName {
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
    STATE_AVALIABLE,
    STATE_NOT_AVALIABLE
  };

  virtual InterfaceState GetInterfaceState(InterfaceName interface) const = 0;
  virtual InterfaceName GetInterfaceFromFunction(
      hmi_apis::FunctionID::eType function) const = 0;
  virtual void SetIntefaceState(InterfaceName interface,
                                InterfaceState state) = 0;
  virtual ~HmiInterfaces() {}
};
}
#endif  // HMI_INTERFACES_H
