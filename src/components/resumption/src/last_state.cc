#include "resumption/last_state.h"

namespace resumption {

LastState* LastState::instance(void) {
  static LastState instance;
  return &instance;
}

const LastState::TcpDeviceStateContainer&
LastState::tcp_device_states(void) const {
  return tcp_device_states_;
}

void LastState::StoreTcpDeviceStates(
  const TcpDeviceStateContainer& tcp_device_states) {
  tcp_device_states_ = tcp_device_states;
}

const LastState::BluetoothDeviceStateContainer&
LastState::bluetooth_device_states(void) const {
  return bluetooth_device_states_;
}

void LastState::StoreBluetoothDeviceStates(
  const BluetoothDeviceStateContainer& bluetooth_device_states) {
  bluetooth_device_states_ = bluetooth_device_states;
}

}
