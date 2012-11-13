* Introduction
  ================
  appLinkCore is an application which manages the transport, connection and communication between a head unit and mobile device.

* OS and Hardware
  =========
  Ubuntu 12.04.1 LTS OS on the PC with USB-dongle
  Application has been tested using 2 types of USB-dongle:
  D-Link DBT-122
  STLab B-121mini

  * External components
  ===================
  For start application we need:
  libbluetooth3, the BlueZ library
  Install library:
  sudo apt-get install libbluetooth3
  To start web-based HMI we need web-browser with web-socket RFC6455 support.
  For example Google Chromium. Install it using:
  sudo apt-get install chromium-browser
  In current implementation Chromium is required for autostart HMI feature.
  For HMI autostart please create in the executable folder file named hmi_link.
  This file should contain one string with full path to HMI index.html file.
  For example:
  /home/user/projects/applink/src/components/HMI/index.html

* Running application
  ====================
  Plug USB-dongle in.
  Switch Bluetooth on a mobile device ON and make the device discoverable.
  Pair mobile device with PC using Ubuntu tools.
  Device should contain AppLink compatible application installed.
  Start application with command:
  ./appLinkCore
  Application starts to search devices and starts HMI in cromium-browser.
  In case HMI has not been started please start web-based HMI manually in browser opening src/components/HMI/index.html.
  Select the device from a list.
  Application starts SDP request.
  Select port.
  Application opens port and starts communication.

