* Introduction
  ================
  smartDeviceCore is an application which manages the transport, connection and communication between a head unit and mobile device.

* OS and Hardware
  =========
  Ubuntu 12.04.1 LTS 32-bit OS on the PC with USB-dongle
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
  For HMI autostart please set path to hmi in config file
  For example:
  LinkToWebHMI = ${path_to_HMI_repository}/index.html

* Running application
  ====================
  Plug USB-dongle in.
  Switch Bluetooth on a mobile device ON and make the device discoverable.
  Pair mobile device with PC using Ubuntu tools.
  Device should contain SmartDeviceLink compatible application installed.
  Start application with command:
  ./smartDeviceLinkCore
  Application starts to search devices and starts HMI in cromium-browser.
  In case HMI has not been started please start web-based HMI manually in browser opening src/components/HMI/index.html.
  SmartDeviceLinkCore is searching Bluetooth devices with a correspondibg service.
  Go to info menu in HMI and press App button.
  Press change Devices button.
  Select the device from a list.
  Application opens all available ports on devices and starts communication.
  Returning to the App menu all applications will be shown in a list.

* Colorized logs
  ==============
  You can have colorized log output of smartDeviceLinkCore's messages in the terminal with the help of grc:

  1. Make sure grc is installed:
  sudo apt-get install grc

  2. Copy the config files from the grc/ directory into ~/.grc/ directory:
  mkdir ~/.grc
  cp grc/* ~/.grc/

  3. Add an alias to your shell's config (usually, ~/.bashrc or ~/.zshrc):
  alias grca='grc -es --colour=auto'

  Either restart the shell session or source the edited file:
  source ~/.bashrc
  or
  source ~/.zshrc

  4. Start the smartDeviceLink core with the following command:
  grca ./smartDeviceLinkCore

  5. PROFIT

