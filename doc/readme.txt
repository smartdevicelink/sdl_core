* Introduction
  ================
  appLinkCore is an application which manages the transport, connection and communication between a head unit and mobile device.

* OS and Hardware
  =========
  Ubuntu 12.04 LTE OS on the PC with USB-dongle
  Application has been tested using 2 types of USB-dongle:
  D-Link DBT-122
  STLab B-121mini

* Running application
  ====================
  Plug USB-dongle in.
  Switch Bluetooth on a mobile device ON and make the device discoverable.
  Pair mobile device with PC using Ubuntu tools.
  Device should contain AppLink compatible application installed.
  Start application with command:
  ./appLinkCore
  Application starts to search devices.
  Select the device from a list.
  Application starts SDP request.
  Select port.
  Application opens port and starts communication.

