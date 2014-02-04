#ifndef SRC_COMPONENTS_RESUMPTION_INCLUDE_RESUMPTION_LAST_STATE_H_
#define SRC_COMPONENTS_RESUMPTION_INCLUDE_RESUMPTION_LAST_STATE_H_

#include <netinet/in.h>
#include <bluetooth/bluetooth.h>

#include <string>
#include <vector>

namespace resumption {

class LastState {
 public:
/**
 * @brief Type definition for application state on TCP device
 */
  typedef struct {
    uint16_t port;
  } TcpDeviceApplicationState;
/**
 * @brief Type definition for container of application states on TCP device
 */
  typedef std::vector<TcpDeviceApplicationState> TcpDeviceApplicationStateContainer;
/**
 * @brief Type definition for TCP device state
 */
  typedef struct {
    in_addr_t in_addr;
    std::string name;
    TcpDeviceApplicationStateContainer applications;
  } TcpDeviceState;
/**
 * @brief Type definition for container of TCP device states
 */
  typedef std::vector<TcpDeviceState> TcpDeviceStateContainer;
/**
 * @brief Type definition for application state on Bluetooth device
 */
  typedef struct {
    uint8_t rfcomm_channel;
  } BluetoothDeviceApplicationState;
/**
 * @brief Type definition for container of application states on Bluetooth device
 */
  typedef std::vector<BluetoothDeviceApplicationState> BluetoothDeviceApplicationStateContainer;
/**
 * @brief Type definition for Bluetooth device state
 */
  typedef struct {
    bdaddr_t address;
    std::string name;
    BluetoothDeviceApplicationStateContainer applications;
  } BluetoothDeviceState;
/**
 * @brief Type definition for container of Bluetooth device states
 */
  typedef std::vector<BluetoothDeviceState> BluetoothDeviceStateContainer;
/**
 * @brief Returns the singleton of class
 */
  static LastState* instance(void);
/**
 * @brief Getter for container of TCP device states
 *
 * @return Constant reference to container of TCP device states
 */
  const TcpDeviceStateContainer& tcp_device_states(void) const;
/**
 * @brief Setter for container of TCP device states
 */
  void StoreTcpDeviceStates(const TcpDeviceStateContainer& tcp_device_states);
/**
 * @brief Getter for container of Bluetooth device states
 *
 * @return Constant reference to container of Bluetooth device states
 */
  const BluetoothDeviceStateContainer& bluetooth_device_states(void) const;
/**
 * @brief Setter for container of Bluetooth device states
 */
  void StoreBluetoothDeviceStates(
    const BluetoothDeviceStateContainer& bluetooth_device_states);

 private:
  TcpDeviceStateContainer tcp_device_states_;
  BluetoothDeviceStateContainer bluetooth_device_states_;
};

}  // namespace resumption

#endif  // SRC_COMPONENTS_RESUMPTION_INCLUDE_RESUMPTION_LAST_STATE_H_
